/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

#include "client.hpp"

Client::Client(int p, std::string address): port_(p)
{
    NetworkManager client(8080, "client");
    asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 8080);

    MoveRequest req;
    req.type = 0x01;
    req.dir = UP;
    client.send(reinterpret_cast<uint8_t*>(&req), sizeof(req), server_endpoint);

    while (1) {
        client.poll();

        std::vector<u_int8_t> msg = client.getLastMsg();
        if (!msg.empty()) {
            MoveResponse *res = reinterpret_cast<MoveResponse*>(msg.data());
            if (res->type == 0x24) {
                std::cout << "Serveur: Player " << res->player_id
                          << " se déplace vers " << res->direction.x << ", " << res->direction.y
                          << std::endl;
            }
        }

        u_int8_t reponse;
        std::string input;
        if (std::getline(std::cin, input)) {
            MoveRequest resq;
            resq.type = 0x23;

            if (input == "up")
                resq.dir = UP;
            if (input == "down")
                resq.dir = DOWN;
            if (input == "left")
                resq.dir = LEFT;
            if (input == "RIGHT")
                resq.dir = RIGHT; 

            client.send(reinterpret_cast<uint8_t*>(&resq), sizeof(resq), server_endpoint);
        }

    }
}

MoveRequest Client::getMoveKey()
{
    return move;
}

Client::~Client()
{

}