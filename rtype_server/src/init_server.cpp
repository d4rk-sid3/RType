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

        if (!msg.first.empty() && msg.first.size() >= sizeof(MoveRequest)) {
            MoveRequest res{};
            std::memcpy(&res, msg.first.data(), sizeof(MoveRequest));
            
            if (res.type == 0x23) {
                std::cout << "Client bouge: direction = " << res.dir << std::endl;
            }

            auto sender = msg.second;

            MoveResponse resp;
            resp.type = 0x24;
            resp.player_id = 1;
            if (res.dir == RIGHT)
                resp.direction = {1, 0};
            
            if (res.dir == LEFT)
                resp.direction = {-1, 0};

            if (res.dir == UP)
                resp.direction = {0, -1};

            if (res.dir == DOWN)
                resp.direction = {0, 1};

            resp.position = { 100, 200 };
            resp.speed = 2.5f;
            resp.timestamp = time(nullptr);

            server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
        } else if (!msg.first.empty()) {
            std::cerr << "Message reçu trop petit: " << msg.first.size() << " bytes" << std::endl;
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
