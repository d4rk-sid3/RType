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

#include "Types.hpp"

class NetworkManager {
  public:

    // Constructeurs and destructeurs
    /**
     * @brief Construct a new Network Manager object for client
     * @param port The port to connect to the server
     * @param address The address of the server
     * @param lastmsg Reference to the last message received from the server
     * @param mtx Mutex for thread safety with the last message
     */
    NetworkManager(int port, std::string address, std::vector<int8_t> &lastmsg_, std::mutex& mtx_);

    /**
     * @brief Construct a new Network Manager object for server
     * @param port The port to listen on
     * @param clients_lastmsg_ Reference to the list of pairs of clients and their last messages
     * @param mtx_ Mutex for thread safety with the clients' last messages
     */
    NetworkManager(int port, std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> &clients_lastmsg_, std::mutex& mtx_);

    /**
     * @brief Destroy the Network Manager object
     */
    ~NetworkManager();

    // Methodes for running the network io_context

    /**
     * @brief Start the context and networking operations
     */
    void run();

    /**
     * @brief Stop the context and networking operations
     */
    void stop();

    /**
     * @brief Receive messages from clients
     */
    void receive_from_clients();

    /**
     * @brief Send a message to a specific client
     * @param msg The message to send
     * @param size The size of the message
     * @param client The endpoint of the client to send the message to
     */
    void send_to_client( const std::vector<int8_t>& msg, size_t size,
        const asio::ip::udp::endpoint& client );

    /**
     * @brief Receive messages from the server
     */
    void receive_from_server();

    /**
     * @brief Send a message to the server
     * @param msg The message to send
     * @param size The size of the message
     */
    void send_to_server( const std::vector<int8_t>& msg, size_t size );

  private:
    /**
     * @brief Context for asynchronous operations
     */
    asio::io_context context;

    /**
     * @brief The UDP socket used for communication
     */
    asio::ip::udp::socket socket;

    /**
     * @brief Buffer for storing incoming messages
     */
    std::array<int8_t, 1024> buff{};

    /**
     * @brief The last sender's endpoint (used for server)
     */
    asio::ip::udp::endpoint last_sender_;

    /**
     * @brief The server's endpoint (used for client)
     */
    asio::ip::udp::endpoint server_endpoint_;

    /**
     * @brief Flag indicating if the network manager is running
     */
    bool isrunning;


    /**
     * @brief List of pairs of clients and their last messages (used for server)
     */
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> tmp_clients;
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> &clients_lastmsg;

    /**
     * @brief Reference to the last message received from the server (used for client)
     */
    std::vector<int8_t> tmp_server;
    std::vector<int8_t> &lastmsg;
  
    std::vector<client_info_t> clients;

    /**
     * @brief Mutex for thread safety
     */
    std::mutex& mtx;
};

#endif /* !NETWORK_HPP_ */
