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

#include <SFML/Graphics.hpp>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>

enum Direction : int16_t { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

enum BULLET_TYPE : int16_t { SOLIDE = 0, NONE = 1 };

struct Vector2D {
    int16_t x;
    int16_t y;
};

enum EnemyType : int16_t { TYPE_1 = 1, TYPE_2 = 2, TYPE_3 = 3, TYPE_4 = 4 };

// Serveur -> Client
struct EnemyMovedResponse {
    int16_t type; // 0x37
    int16_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
};

// Serveur -> Client
struct NbrEntity {
    int16_t type; // 0x38
    int16_t nbr;
};

struct MoveResponse {
    int16_t type; // 0x24
    int16_t player_id;
    Direction direction;
};


class NetworkManager {
  public:
    NetworkManager(int port, std::string address, std::vector<int16_t> &lastmsg_, std::mutex& mtx_);

    NetworkManager(int port, std::vector<int16_t> &lastmsg_, std::mutex& mtx_);

    ~NetworkManager();

    void poll();
    void run();
    void receive();
    void send(
        const std::vector<int16_t>& msg, size_t size,
        const asio::ip::udp::endpoint& client
    );
    asio::ip::udp::endpoint getLastSender() const;
    asio::io_context& getContext() {
        return context;
    };

    asio::ip::udp::endpoint& getServerendpoint() {
        return server_endpoint_;
    };

    void add_connection(const asio::ip::udp::endpoint& ep);

  protected:
  private:
    asio::io_context context;
    asio::ip::udp::socket socket;
    std::array<int16_t, 1024> buff{};
    asio::ip::udp::endpoint last_sender_;
    asio::ip::udp::endpoint server_endpoint_;
    bool isrunning;
    std::vector<int16_t>& lastmsg;
    std::vector<asio::ip::udp::endpoint> clients;
    std::mutex& mtx;
};

#endif /* !NETWORK_HPP_ */
