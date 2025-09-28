/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "../include/server.hpp"

Server::Server(int p): p_(p), server_(8080, "127.0.0.1")
{
    std::vector<asio::ip::udp::endpoint> client_endpoint;

    while (1) {
        server_.poll();

        auto msg = server_.getLastMsg();

        if (!msg.first.empty()) {
            u_int8_t type = msg.first[0];
            auto sender = msg.second;

            if (type == 0x23 && msg.first.size() >= sizeof(MoveRequest)) {
                MoveRequest res{};
                MoveResponse resp = recupMove(msg, res);
                server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
            } else if (type == 0x27) {
                PickupItemResponse resp = recupItem(msg);
                server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
            } else if (type == 0x28) {
                PlayerStateResponse resp = player_r(msg);
                server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
            } else if (type == 0x29) {
                PlayerStateResponse2 resp = player_r2(msg);
                server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
            } else {
                std::cerr << "Message inconnue: " << (int)type
                    << "size= " <<  msg.first.size() << " bytes" << std::endl;
            }
        }
    }
}

MoveResponse Server::getMove()
{
    return response;
}

Server::~Server()
{

}
