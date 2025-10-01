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
                resq.direction = UP;
            else if (input == "down")
                resq.direction = DOWN;
            else if (input == "left")
                resq.direction = LEFT;
            else if (input == "right")
                resq.direction = RIGHT; 
            else
                continue;
            
            std::vector<uint8_t> buff = encodeMoveResquest(resq);

            this->client_.send(buff, buff.size(), server_endpoint);
        }
    });

    while (1) {

        client_.poll();

        auto msg = client_.getLastMsg();
        if (!msg.first.empty()) {
            MoveResponse res = decodeMoveResponse(msg.first);
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