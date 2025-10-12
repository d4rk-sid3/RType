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
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <time.h>
#include <vector>
#include <queue>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    // Alias pour compatibilité
    typedef uint8_t u_int8_t;
    typedef uint16_t u_int16_t;
    typedef uint32_t u_int32_t;
    typedef uint64_t u_int64_t;
#else
    #include <poll.h>
    #include <arpa/inet.h>
    #include <netinet/ip.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif
#include <SFML/Graphics.hpp>

#include "Types.hpp"

class NetworkManager {
  public:

    // Constructeurs and destructeurs
    NetworkManager(int port, std::string address, std::vector<int8_t> &lastmsg_, std::mutex& mtx_);
    NetworkManager(int port, std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> &clients_lastmsg_, std::mutex& mtx_);
    ~NetworkManager();

    // Methodes for running the network io_context
    void run();

    void receive_from_clients();
    void send_to_client( const std::vector<int8_t>& msg, size_t size,
        const asio::ip::udp::endpoint& client );

    void receive_from_server();
    void send_to_server( const std::vector<int8_t>& msg, size_t size );

    // Getters
    asio::ip::udp::endpoint getLastSender() const;
    asio::io_context& getContext();
    asio::ip::udp::endpoint& getServerendpoint();

  private:
    asio::io_context context;
    asio::ip::udp::socket socket;
    std::array<int8_t, 1024> buff{};
    asio::ip::udp::endpoint last_sender_;
    asio::ip::udp::endpoint server_endpoint_;
    bool isrunning;


    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> tmp_clients;
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> &clients_lastmsg;

    std::vector<int8_t> tmp_server;
    std::vector<int8_t> &lastmsg;
  
    std::vector<client_info_t> clients;
    std::mutex& mtx;
};

#endif /* !NETWORK_HPP_ */
