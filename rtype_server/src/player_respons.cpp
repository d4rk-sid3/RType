/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** player_respons
*/

#include "../include/server.hpp"

PlayerStateResponse Server::player_r(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a)
{
    PlayerStateResponse r;
    r.type = 0x28;
    r.player_id = 1;
    r.remaining_health = 50;
    r.score = 20;
    r.current_level = 2;

    return r;
}

PlayerStateResponse2 Server::player_r2(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a)
{
    PlayerStateResponse2 r;
    r.type = 0x29;
    r.player_id = 1;
    r.remaining_health = 50;
    r.score = 20;
    r.current_level = 2;

    return r;
}
