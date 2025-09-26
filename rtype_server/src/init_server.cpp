/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "../include/server.hpp"

Server::Server(int p) : p_(p) {
    NetworkManager server(8080, "127.0.0.1");
    asio::ip::udp::endpoint client_endpoint;
    bool client_valid = false;

    while (1) {
        server.poll();

        std::string msg = server.getLastMsg();
        if (!msg.empty()) {
            client_endpoint = server.getLastSender();
            client_valid = true;
            std::cout << "Client: " << msg << std::endl;

            std::string input;
            if (std::getline(std::cin, input) && !input.empty()) {
                if (client_valid)
                    server.send(input, client_endpoint);
            }
        }
    }
}

Server::~Server() {}