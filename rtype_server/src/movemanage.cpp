/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** movemanage
*/

#include "../include/server.hpp"

MoveResponse Server::recupMove(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a, MoveRequest &m)
{
    std::memcpy(&m, a.first.data(), sizeof(MoveRequest));
    
    if (m.type == 0x23) {
        std::cout << "Client bouge: direction = " << m.dir << std::endl;
    }

    MoveResponse r;
    r.type = 0x24;
    r.player_id = 1;
    if (m.dir == RIGHT)
        r.direction = {1, 0};
    if (m.dir == LEFT)
        r.direction = {-1, 0};
    if (m.dir == UP)
        r.direction = {0, -1};
    if (m.dir == DOWN)
        r.direction = {0, 1};

    r.position = { 100, 200 };
    r.speed = 2.5f;
    r.timestamp = time(nullptr);

    return r;
}
