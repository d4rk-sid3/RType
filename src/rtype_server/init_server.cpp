/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "header/Network.hpp"

NetworkManager::NetworkManager(int port): socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), isrunning(true)
{
    std::cout << "Serveur pret à être lancé" << std::endl;
    Receive();
}

NetworkManager::~NetworkManager()
{
    isrunning = false;
    socket.close();
}

void NetworkManager::run()
{
    if (isrunning) {
        context.run();
    }
}

void NetworkManager::Receive()
{
    socket.async_receive_from(asio::buffer(buff), client_, 
        
        [this](std::error_code error ,std::size_t bytes_receive) {

            std::string data(buff.data(), bytes_receive);
                std::cout << "Reçu: " << data << std::endl;

            if (!error && bytes_receive > 0) {
                Sendit("Recieve", client_);
            }

            if (isrunning) {
                Receive();
            }
        
        }
    );

}

void NetworkManager::Sendit(const std::string &msg, const asio::ip::udp::endpoint& client)
{
    socket.async_send_to(asio::buffer(msg), client, 
        [this](std::error_code error, std::size_t byte_send) {
            
            if (!error) {
                std::cout << "Send  " << byte_send <<  std::endl;
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}
