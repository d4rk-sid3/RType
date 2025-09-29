#include <iostream>
#include <string>
#include <array>
#include <ctime>
#include <sodium.h>

class User_Stats {
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
        using Token128 = std::array<uint8_t,16>;
        using Token256 = std::array<uint8_t,32>;

        User(size_t id, std::string username, std::string plain_password)
        : id_(id), username_(std::move(username)) 
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
        void setAuthToken(const Token256& tok) {
            _auth_token = tok;
            _auth_token_hex = bytesToHex(auth_token.data(), auth_token.size());
        }
        void setSessionToken(const Token128& tok) {
            _session_token = tok;
            _session_token_hex = bytesToHex(_session_token.data(), _session_token.size());
        }
        void setClientHash(const Token256& ch) {
            _client_hash = ch;
        }
        void setName(const std::string &name) {
            _username = name;
        }
        const std::string& getUsername() const {return _username;}
        const std::string& getAuthTokenHex() const { return auth_token_hex_; }

    private:
        size_t _id = 0;
        std::string _username;
        std::string _password_hash;
        Token256 _auth_token{};
        Token128 _session_token{};
        std::string _auth_token_hex;
        std::string _session_token_hex;
        std::time_t _last_login_at = 0;
        Token256 _client_hash;
        User_Stats _stats;
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

        
        
};
#endif /* defined(_Game_) */