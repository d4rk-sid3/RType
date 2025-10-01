/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "../include/server.hpp"

Server::Server(int p): p_(p), server_(8080, "127.0.0.1")
{
    response = {};
    response.type = 0x24;
    response.player_id = 1;
    response.position = {100, 200};
    response.direction = {0, 0};
    response.speed = 0;
    response.timestamp.milliseconds = 0;

    while (1) {
        server_.poll();

        auto msg = server_.getLastMsg();

        if (!msg.first.empty()) {
            u_int8_t type = msg.first[0];
            auto sender = msg.second;

            if (type == 0x23) {
                MoveRequest req = decodeMoveRequest(msg.first);
                std::cout << "Client direction = " << static_cast<int>(req.direction) << std::endl;

                MoveResponse newResp = response;
                newResp.type = 0x24;
                newResp.player_id = 1;
                newResp.speed = 2;
                newResp.timestamp.milliseconds =
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();

                switch (req.direction) {
                    case UP:
                        newResp.direction = {0, static_cast<uint16_t>(-1)};
                        break;
                    case DOWN:
                        newResp.direction = {0, 1};
                        break;
                    case LEFT:
                        newResp.direction = {static_cast<uint16_t>(-1), 0};
                        break;
                    case RIGHT:
                        newResp.direction = {1, 0};
                        break;
                }

                response = updateMoveResponse(response, newResp);


                std::vector<uint8_t> buff = encodeMoveResponse(response);
                server_.send(buff, buff.size(), sender);
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
