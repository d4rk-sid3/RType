/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include "Network.hpp"

class Client {
  private:
    int port_;

  public:
    Client(int p, std::string a);
    ~Client();
};

#endif /* !CLIENT_HPP_ */
