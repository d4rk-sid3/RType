/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "server.hpp"

Server::Server(int p): p_(p)
{
    NetworkManager server(8080, "127.0.0.1");
    std::vector<asio::ip::udp::endpoint> client_endpoint;

    while (1) {
        server.poll();

        std::vector<uint8_t> msg = server.getLastMsg();

        if (!msg.empty()) {
            MoveRequest *res = reinterpret_cast<MoveRequest*>(msg.data());
            
            if (res->type == 0x23) {
                std::cout << "Client bouge: direction=" << res->dir << std::endl;
            }

            MoveResponse resp;
            resp.type = 0x24;
            resp.player_id = 1;
            resp.direction = { (res->dir == RIGHT) - (res->dir == LEFT),
                                (res->dir == DOWN) - (res->dir == UP) };

            resp.position = { 100, 200 };
            resp.speed = 2.5f;
            resp.timestamp = time(nullptr);

            client_endpoint.push_back(server.getLastSender());
            for (auto& c : client_endpoint) {
                server.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), c);
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
