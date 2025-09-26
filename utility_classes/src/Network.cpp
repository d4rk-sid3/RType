/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include "../include/Network.hpp"

NetworkManager::NetworkManager(int port, std::string address)
    : socket(context), isrunning(true) {
    socket.open(asio::ip::udp::v4());

    if (address == "127.0.0.1") {
        socket.set_option(asio::socket_base::reuse_address(true));
        socket.bind(
            asio::ip::udp::endpoint(asio::ip::make_address("127.0.0.1"), port)
        );
        std::cout << "Serveur pret à être lancé " << port << std::endl;
    } else {
        socket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
        std::cout << "Client lancé " << std::endl;
    }

    receive();
}

NetworkManager::~NetworkManager() {
    isrunning = false;
    socket.close();
}

void NetworkManager::poll() {
    if (isrunning) {
        context.poll();
    }
}

void NetworkManager::receive() {
    socket.async_receive_from(
        asio::buffer(buff), last_sender_,

        [this](std::error_code error, std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                lastmsg.assign(buff.data(), bytes_receive);
            }

            if (isrunning) {
                receive();
            }
        }
    );
}

void NetworkManager::send(
    const std::string& msg, const asio::ip::udp::endpoint& client
) {
    socket.async_send_to(
        asio::buffer(msg), client,
        [this](std::error_code error, std::size_t byte_send) {
            if (!error) {
                std::cout << "Send  " << byte_send << std::endl;
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}

std::string NetworkManager::getLastMsg() {
    std::string tmp = lastmsg;
    lastmsg.clear();
    return tmp;
}

asio::ip::udp::endpoint NetworkManager::getLastSender() const {
    return last_sender_;
}
