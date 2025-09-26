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

  public:
    Server(int p);
    MoveResponse getMove();
    ~Server();
};

#endif /* !SERVER_HPP_ */
