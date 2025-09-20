/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "header/server.hpp"

Server::Server(int _port)
{
    port = _port;
    asio::io_context context; //création des évènements

    asio::ip::udp::socket socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)); // création des sockets

    std::cout << "Serveur lancé" << std::endl;

    while (1) {
        char buff[1024];

        asio::ip::udp::endpoint client; // adresse ip + port du client

        size_t l = socket.receive_from(asio::buffer(buff), client); // reception des informations envoyé du serveur
        std::cout << "Reçu: " << std::string(buff, l) << " de " << client << std::endl;

        std::string msg = "J'ai reçue" + std::string(buff, l);
        socket.send_to(asio::buffer(msg), client); // envoie du message par le client
    }
}

Server::~Server() {

}