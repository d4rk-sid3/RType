#include "../include/GamePlayer.hpp"

GamePlayer::GamePlayer(const std::string &username, const udp::endpoint &endpoint) : 
    _username(username), _endpoint(endpoint)
{
}

GamePlayer::~GamePlayer(){}

const std::string &GamePlayer::getUsername() const
{
    return _username;
}

const udp::endpoint &GamePlayer::getEndpoint() const
{
    return _endpoint;
}