/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "Network.hpp"

class Server {
  private:
    int p_;
    MoveResponse response;
    MoveRequest move;
    NetworkManager server_;
    PickupItemResponse item;
    PlayerStateResponse p_response;
    PlayerStateResponse2 p_response2;
 
  public:
    Server(int p);
    MoveResponse getMove();
    MoveResponse recupMove(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a, MoveRequest &m);

    PickupItemResponse recupItem(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);
    PlayerStateResponse player_r(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);
    PlayerStateResponse2 player_r2(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);

    ~Server();
};

#endif /* !SERVER_HPP_ */
