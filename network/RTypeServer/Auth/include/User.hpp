#ifndef USER
    #define USER
    #include "./IToken.hpp"

class UserStats {
    private:
        int _games_played = 0;
        int _games_won = 0;
        int _level = 0;

    public:
        UserStats();
        ~UserStats();

        int getNbGamesPlayed() const;
        int getNbGamesWon() const;
        int getLevel() const;

        void setNbGamesPlayed(int games_played);
        void setNbGamesWon(int games_won);
        void setLevel(int level);

        void updateGamesPlayed();
        void updateGamesWon();
        void updateLevel();

};

class User {
    public:
        User();
        User(size_t id, std::string username, std::string plain_password);
        void setName(const std::string &name);
        void setAuthToken(const std::shared_ptr<IToken> token);
        void setSessionToken(const std::shared_ptr<IToken> token);
        void setClientHash(const std::string &client_hash);
        void setId(const size_t &id);
        void setPasswordHash(const std::string &password_hash);
        void setNbGamesPlayed(const int games_played);
        void setNbGamesWon(const int games_won);
        const std::string getClientHash();
        const std::weak_ptr<IToken> getAuthToken();
        const std::weak_ptr<IToken> getSessionToken();
        const std::string& getAuthTokenHex() const;
        const std::string& getUsername() const;
        const std::string& getPasswordHash() const;
        size_t getId() const;
        void setLastLogin(const std::time_t time);
        void save(sqlite3* db) const;

    private:
        size_t _id = 0;
        std::string _username;
        std::string _password_hash;
        std::weak_ptr<IToken> _auth_token;
        std::weak_ptr<IToken> _session_token;
        std::time_t _last_login_at = 0;
        std::string _client_hash = "";
        UserStats _stats;
};
#endif