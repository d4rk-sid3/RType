/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

#include "../include/client.hpp"

Client::Client(int p, std::string address): port_(p), client_(8080, "client")
{
    asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 8080);

    std::thread input_thread([this, server_endpoint]() {
        std::string input;
        while (std::getline(std::cin, input)) {
            MoveRequest resq;
            resq.type = 0x23;

            if (input == "up")
                resq.dir = UP;
            else if (input == "down")
                resq.dir = DOWN;
            else if (input == "left")
                resq.dir = LEFT;
            else if (input == "right")
                resq.dir = RIGHT; 
            else
                continue;

            this->client_.send(reinterpret_cast<uint8_t*>(&resq), sizeof(resq), server_endpoint);
        }
    });

    while (1) {

        client_.poll();

        auto msg = client_.getLastMsg();
        if (!msg.first.empty()) {
            MoveResponse res{};
            std::memcpy(&res, msg.first.data(), sizeof(MoveResponse));
            if (res.type == 0x24) {
                std::cout << "Serveur: Player " << res.player_id
                        << " se déplace vers " << res.direction.x << ", " << res.direction.y
                        << std::endl;
            }
        }
    }
    input_thread.detach();
}

MoveRequest Client::getMoveKey()
{
    return move;
}

Client::~Client()
{

}