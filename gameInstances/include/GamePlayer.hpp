#pragma once
#include "../../Auth/include/librairies.hpp"

class GamePlayer {
    private:
        std::string _username;
        udp::endpoint _endpoint;
    public:
        GamePlayer(const std::string &username, const udp::endpoint &endpoint);
        ~GamePlayer();
        const std::string &getUsername() const;
        const udp::endpoint &getEndpoint() const;
};
