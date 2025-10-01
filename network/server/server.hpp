#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <string>
#include <array>
#include <ctime>
#include <sodium.h>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstring>
#include <random>


using Clock = std::chrono::steady_clock;

enum class TokenType {
    AUTH,
    SESSION
};

class IToken {
    public:
        virtual ~IToken() = default;
        virtual TokenType getType() const = 0;
        virtual bool isExpired() const = 0;
        virtual void createToken(const uint8_t* key, size_t key_len,
            const void* message, size_t message_len) = 0;
        virtual const std::string& getHex() const = 0;
        virtual uint64_t getTTL() const = 0;
        
        template <size_t N>
        static std::array<uint8_t, N> fromHex(const std::string& hex) {
            if (hex.size() != N * 2) {
                throw std::runtime_error("Invalid hex string length for token");
            }
            std::array<uint8_t, N> out{};
            for (size_t i = 0; i < N; i++) {
                std::string byteString = hex.substr(i * 2, 2);
                out[i] = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
            }
            return out;
        }
        template <size_t N>
        static std::string toHex(const std::array<uint8_t, N> &token) {
            std::stringstream ss;

            for (size_t i = 0; i < N; i++) {
                ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(token[i]);
            }
            return ss.str();
        }
};

class Token256 : public IToken {
    using Data = std::array<uint8_t, 32>;

    TokenType _type;
    Clock::time_point _created_at;
    std::chrono::seconds _ttl;
    Data _data;
    std::string _token_hex;

public:
    Token256(TokenType type = TokenType::AUTH, uint64_t ttl_seconds = 86400)
        : _type(type),
          _created_at(Clock::now()),
          _ttl(std::chrono::seconds(ttl_seconds))
    {
    }
    
    TokenType getType() const override { return _type; }

    bool isExpired() const override {
        return Clock::now() - _created_at > _ttl;
    }

    uint64_t getTTL() const override { return _ttl.count(); }

    void createToken(const uint8_t *key, size_t key_len,
        const void* message, size_t message_len) override
    {
        crypto_auth_hmacsha256(_data.data(), static_cast<const uint8_t*>(message), message_len, key);
        _token_hex = IToken::toHex<32>(_data);
        return;
    }

    const Data& getData() const { return _data; }
    void setData(const Data& data) {
        _data = data;
        _token_hex = IToken::toHex<32>(data);
    }
    const std::string& getHex() const { return _token_hex; }
};

class Token128 : public IToken {
    public:
        using Data = std::array<uint8_t, 16>;
    
    private:
        TokenType _type;
        Clock::time_point _created_at;
        std::chrono::seconds _ttl;
        Data _data;
        std::string _token_hex;
    
    public:
        Token128(TokenType type = TokenType::SESSION, uint64_t ttl_seconds = 86400)
            : _type(type),
              _created_at(Clock::now()),
              _ttl(std::chrono::seconds(ttl_seconds))
        {
        }    
        TokenType getType() const override { return _type; }
    
        bool isExpired() const override {
            return Clock::now() - _created_at > _ttl;
        }
    
        uint64_t getTTL() const override { return _ttl.count(); }
    
        void createToken(const uint8_t *key, size_t key_len,
            const void* message, size_t message_len) override
        {
            std::array<uint8_t, 32> full_hmac;
            crypto_auth_hmacsha256(full_hmac.data(), static_cast<const uint8_t*>(message),
                message_len, key);
            std::copy(full_hmac.begin(), full_hmac.begin() + 16, _data.begin());    
            _token_hex = IToken::toHex<16>(_data);
        }
        const Data& getData() const { return _data; }
        void setData(const Data& data) {
            _data = data;
            _token_hex = IToken::toHex<16>(data);
        }
        const std::string& getHex() const { return _token_hex; }
    };

    class TokenManager {
        private:
            std::unordered_map<size_t, std::shared_ptr<IToken>> _tokens;
            std::unordered_map<std::string, size_t> _hex_tokens;
            size_t _next_id = 1;
        
            static TokenManager* s_pInstance;
            TokenManager() = default;
        
        public:
            ~TokenManager() = default;
        
            static TokenManager* Instance() {
                if (!s_pInstance) {
                    s_pInstance = new TokenManager();
                }
                return s_pInstance;
            }
        
            TokenManager(const TokenManager&) = delete;
            TokenManager& operator=(const TokenManager&) = delete;
        
            std::shared_ptr<IToken> createToken(TokenType type,
                const uint8_t* key, size_t key_len,
                const void* message, size_t message_len)
            {
                std::shared_ptr<IToken> token;
        
                if (type == TokenType::AUTH) {
                    token = std::make_shared<Token256>(type);
                } else {
                    token = std::make_shared<Token128>(type);
                }
                token->createToken(key, key_len, message, message_len);
        
                _tokens[_next_id] = token;
                _hex_tokens[token->getHex()] = _next_id;
                
                _next_id++;
                return token;
            }

            void deleteToken(size_t id)
            {
                auto it = _tokens.find(id);
                if (it != _tokens.end()) {
                    _hex_tokens.erase(it->second->getHex());
                    _tokens.erase(it);
                }
            }
        
            bool verifyToken(const std::string& hex) const {
                return _hex_tokens.count(hex) > 0;
            }
        
            void update() {
                std::vector<size_t> expired_ids;
                for (auto& [id, token] : _tokens) {
                    if (token->isExpired())
                        expired_ids.push_back(id);
                }
                for (size_t id : expired_ids) {
                    deleteToken(id);
                }
            }

            std::shared_ptr<Token256> generateAuthToken(size_t player_id, const uint8_t server_key[32])
            {
                auto token = std::make_shared<Token256>(TokenType::AUTH);

                uint8_t message[sizeof(player_id) + 24];
                std::memcpy(message, &player_id, sizeof(player_id));

                std::random_device rd;
                for (size_t i = 0; i < 24; i++) {
                    message[sizeof(player_id) + i] = static_cast<uint8_t>(rd() & 0xFF);
                }
                token->createToken(server_key, 32, message, sizeof(message));

                size_t id = _next_id++;
                _tokens[id] = token;
                _hex_tokens[token->getHex()] = id;

                return token;
            }

            std::shared_ptr<Token128> generateSessionToken(size_t player_id, const uint8_t server_key[32])
            {
                auto auth_token = generateAuthToken(player_id, server_key);
                auto session_token = std::make_shared<Token128>(TokenType::SESSION);

                Token128::Data session_data{};
                std::copy(auth_token->getData().begin(),
                        auth_token->getData().begin() + 16,
                        session_data.begin());

                session_token->setData(session_data);

                size_t id = _next_id++;
                _tokens[id] = session_token;
                _hex_tokens[session_token->getHex()] = id;

                return session_token;
            }
};
        
class UserStats {
    private:
        int _games_played = 0;
        int _games_won = 0;

    public:
        UserStats() = default;
        ~UserStats(){};

        int getNbGamesPlayed() const { return _games_played; }
        int getNbGamesWon() const { return _games_won; }

        void setNbGamesPlayed(int games_played) { _games_played = games_played; }
        void setNbGamesWon(int games_won) { _games_won = games_won; }

        void updateGamesPlayed() { _games_played++; }
        void updateGamesWon() { _games_won++; }
};

class User {
    public:
        User(size_t id, std::string username, std::string plain_password)
        : _id(id), _username(std::move(username)) 
        {
            if (sodium_init() < 0) {
                throw std::runtime_error("Failed to initialize libsodium");
            }
            char hashed_password[crypto_pwhash_STRBYTES];
            sodium_mlock(const_cast<char*>(plain_password.data()), plain_password.size());

            if (crypto_pwhash_str(
                    hashed_password,
                    plain_password.c_str(),
                    plain_password.size(),
                    crypto_pwhash_OPSLIMIT_MODERATE,
                    crypto_pwhash_MEMLIMIT_MODERATE) != 0)
            {
                sodium_munlock(const_cast<char*>(plain_password.data()), plain_password.size());
                throw std::runtime_error("Failed to hash password");
            }
            sodium_munlock(const_cast<char*>(plain_password.data()), plain_password.size());
            sodium_memzero(const_cast<char*>(plain_password.data()), plain_password.size());
            _password_hash = hashed_password;
        }
        void setName(const std::string &name)
        {
            _username = name;
        }
        void setAuthToken(const std::shared_ptr<IToken> token) {
            _auth_token = token;
        }
        void setSessionToken(const std::shared_ptr<IToken> token) {
            _session_token = token;
        }
        void setClientHash(const std::string &client_hash) {
            _client_hash = client_hash;
        }
        const std::string getClientHash() { return _client_hash; }
        const std::shared_ptr<IToken> getAuthToken() { return _auth_token; }
        const std::shared_ptr<IToken> getSessionToken() { return _session_token; }
        const std::string& getAuthTokenHex() const { return _auth_token->getHex(); }
        const std::string& getUsername() const { return _username; }
        const std::string& getPasswordHash() const { return _password_hash; }
        size_t getId() const { return _id; }
        void setLastLogin(const std::time_t time) {_last_login_at = time;}

    private:
        size_t _id = 0;
        std::string _username;
        std::string _password_hash;
        std::shared_ptr<IToken> _auth_token;
        std::shared_ptr<IToken> _session_token;
        std::time_t _last_login_at = 0;
        std::string _client_hash;
        UserStats _stats;

};

class UserManager {
    private:
        std::unordered_map<size_t, User> _users;                  
        std::unordered_map<std::string, size_t> _username_index;
        std::unordered_map<std::string, size_t> _auth_index;
        size_t _next_id = 1;

        static UserManager* s_pInstance;
        UserManager(){};
    public:
        ~UserManager(){};
        static UserManager* Instance() {
            if (s_pInstance == nullptr) {
                s_pInstance = new UserManager();
            }
            return s_pInstance;
        }
        UserManager(const UserManager&) = delete;
        UserManager& operator=(const UserManager&) = delete;

        size_t createUser(const std::string& username, const std::string& plain_password)
        {
            if (_username_index.count(username) != 0)
                throw std::runtime_error("username already exists");
            size_t id = _next_id++;
            User u(id, username, plain_password);
            _users.emplace(id, u);
            _username_index[username] = id;
            return id;
        }

        bool removeUser(size_t id) {
            auto it = _users.find(id);
            if (it == _users.end())
                return false;
            _username_index.erase(it->second.getUsername());
            if (!it->second.getAuthTokenHex().empty())
                _auth_index.erase(it->second.getAuthTokenHex());
            _users.erase(it);
            return true;
        }

        std::optional<User> getUser(size_t id) const {
            auto it = _users.find(id);
            if (it == _users.end())
                return std::nullopt;
            return it->second;
        }

        User* getUserRef(size_t id) {
            auto it = _users.find(id);
            if (it == _users.end())
                return nullptr;
            return &(it->second);
        }
    
        std::optional<std::reference_wrapper<User>> getUserByUsername(const std::string& username)
        {
            auto it = _username_index.find(username);
            if (it == _username_index.end())
                return std::nullopt;
        
            User* u = getUserRef(it->second);
            if (!u)
                return std::nullopt;
            return std::ref(*u);
        }

        std::optional<User> getUserByAuthTokenHex(const std::string& auth_hex) const {
            auto it = _auth_index.find(auth_hex);
            if (it == _auth_index.end())
                return std::nullopt;
            return getUser(it->second);
        }

        void assignAuthToken(size_t id, std::shared_ptr<IToken> token)
        {
            User* u = getUserRef(id);
            if (!u)
                throw std::runtime_error("user not found");        
            if (u->getAuthToken() && !u->getAuthTokenHex().empty()) {
                _auth_index.erase(u->getAuthTokenHex());
            }
            u->setAuthToken(token);  
            _auth_index[u->getAuthTokenHex()] = id;
        }
        
        void assignSessionToken(size_t id, std::shared_ptr<IToken> token) {
            User* u = getUserRef(id);
            if (!u)
                throw std::runtime_error("user not found");
        
            u->setSessionToken(token);
        }

        bool updateUser(const User& user) {
            auto it = _users.find(user.getId());
            if (it == _users.end())
                return false;
            if (it->second.getUsername() != user.getUsername()) {
                _username_index.erase(it->second.getUsername());
                if (_username_index.count(user.getUsername()))
                    throw std::runtime_error("username already exists");
                _username_index[user.getUsername()] = user.getId();
            }
            if (it->second.getAuthTokenHex() != user.getAuthTokenHex()) {
                if (!it->second.getAuthTokenHex().empty())
                    _auth_index.erase(it->second.getAuthTokenHex());
                if (!user.getAuthTokenHex().empty())
                    _auth_index[user.getAuthTokenHex()] = user.getId();
            }
            it->second = user;
            return true;
        }

        std::vector<User> listUsers() const {
            std::vector<User> out;
            out.reserve(_users.size());
            for (const auto& kv : _users)
                out.push_back(kv.second);
            return out;
        }

        void generateAuthTokenAssign(size_t id, const uint8_t server_key[32])
        {
            assignAuthToken(id, TokenManager::Instance()->generateAuthToken(id, server_key));
            return;
        }
        
        void generateSessionTokenAssign(size_t id, const uint8_t server_key[32])
        {
            assignAuthToken(id, TokenManager::Instance()->generateSessionToken(id, server_key));
            return;
        }

        bool authenticate(const std::string &username, const std::string &plain_password, const uint8_t server_key[32])
        {
            auto userOpt = getUserByUsername(username);
            if (!userOpt)
                return false;

            User& u = userOpt.value().get();

            const std::string& stored_hash = u.getPasswordHash();
            if (crypto_pwhash_str_verify(stored_hash.c_str(),
                                        plain_password.c_str(),
                                        plain_password.size()) != 0) {
                return false;
            }

            u.setLastLogin(std::time(nullptr));
            generateAuthTokenAssign(u.getId(), server_key);
            return true;
        }

};

#endif /* defined(_Game_) */