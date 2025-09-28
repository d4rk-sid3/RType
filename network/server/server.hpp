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
        int getNbGamesPlayed() const { return _games_played; }
        int getNbGamesWon() const { return _games_won; }

        void setNbGamesPlayed(int games_played) { _games_played = games_played; }
        void setNbGamesWon(int games_won) { _games_won = games_won; }

        void updateGamesPlayed() { _games_played++; }
        void updateGamesWon() { _games_won++; }
};

class User {
    private:
        size_t id;
        std::string username;
        std::string password_hash;
        std::array<uint8_t, crypto_auth_hmacsha256_BYTES> auth_token{};
        std::array<uint8_t, crypto_auth_hmacsha256_BYTES> session_token{};
        std::string auth_token_hex;
        std::string session_token_hex;
        time_t last_login_at = 0;
        std::string client_hash;
        User_Stats stats;

    public:
        size_t getId() const { return id; }
        void setId(size_t uid) { id = uid; }
        const std::string& getUsername() const { return username; }
        void setUsername(const std::string& uname) { username = uname; }
        const std::string& getPasswordHash() const { return password_hash; }
        void setPasswordHash(const std::string& hash) { password_hash = hash; }
        const std::string& getAuthTokenHex() const { return auth_token_hex; }
        void setAuthTokenHex(const std::string& token) { auth_token_hex = token; }
        const std::string& getSessionTokenHex() const { return session_token_hex; }
        void setSessionTokenHex(const std::string& token) { session_token_hex = token; }
        time_t getLastLoginAt() const { return last_login_at; }
        void setLastLoginAt(time_t t) { last_login_at = t; }
        const std::string& getClientHash() const { return client_hash; }
        void setClientHash(const std::string& h) { client_hash = h; }
        User_Stats& getStats() { return stats; }
};
