/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** pickupitem
*/

#include "../include/server.hpp"

PickupItemResponse Server::recupItem(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a)
{
    PickupItemResponse r;
    r.type = 0x27;
    r.player_id = 1;
    r.item_id = 1;
    r.item_position = { 300, 200 };
    r.timestamp = time(nullptr);

    return r;
}
