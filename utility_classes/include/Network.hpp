/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_
#include <asio.hpp>
#include <cstdlib>
#include <ctype.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <limits.h>
#include <poll.h>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>

class NetworkManager {
  public:
    NetworkManager(int port, std::string address = "");
    ~NetworkManager();
    void poll();
    void receive();
    void send(const std::string& msg, const asio::ip::udp::endpoint& client);
    std::string getLastMsg();
    asio::ip::udp::endpoint getLastSender() const;

  protected:
  private:
    asio::io_context context;
    asio::ip::udp::socket socket;
    std::array<char, 1024> buff{};
    asio::ip::udp::endpoint last_sender_;
    bool isrunning;
    std::string lastmsg;
};

#endif /* !NETWORK_HPP_ */
