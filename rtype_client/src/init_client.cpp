/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

#include "../include/client.hpp"

Client::Client(int p, std::string address) : port_(p) {
    NetworkManager client(8080, "client");
    asio::ip::udp::endpoint server_endpoint(
        asio::ip::make_address("127.0.0.1"), 8080
    );

    client.send("Hello", server_endpoint);

    while (1) {
        client.poll();

        std::string msg = client.getLastMsg();
        if (!msg.empty())
            std::cout << "Serveur: " << msg << std::endl;

        std::string reponse;
        if (std::getline(std::cin, reponse) && !reponse.empty())
            client.send(reponse, server_endpoint);
    }
}

Client::~Client() {}
