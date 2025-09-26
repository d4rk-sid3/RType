/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include "../include/Network.hpp"

NetworkManager::NetworkManager(int port, std::string address): socket(context), isrunning(true)
{
    socket.open(asio::ip::udp::v4());
    
    if (address == "127.0.0.1") {
        socket.set_option(asio::socket_base::reuse_address(true));
        socket.bind(asio::ip::udp::endpoint(asio::ip::make_address("127.0.0.1"), port));
        std::cout << "Serveur pret à être lancé " << port << std::endl;
    } else {
        socket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
        std::cout << "Client lancé " << std::endl;
    }

    receive();
    thread_ = std::thread([this]{ context.run(); });
}

NetworkManager::~NetworkManager()
{
    isrunning = false;
    context.stop();
    if (thread_.joinable())
        thread_.join();
    socket.close();

}

void NetworkManager::poll()
{
    if (isrunning) {
        context.poll();
    }
}

void NetworkManager::receive()
{
    socket.async_receive_from(asio::buffer(buff), last_sender_, 
        
        [this](std::error_code error ,std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                messages.push({std::vector<uint8_t>(buff.begin(), buff.begin() + bytes_receive), last_sender_});
            }

            if (isrunning) {
                receive();
            }
        
        }
    );

}

void NetworkManager::send(const u_int8_t *msg, size_t size, const asio::ip::udp::endpoint& client)
{
    socket.async_send_to(asio::buffer(msg, size), client, 
        [this](std::error_code error, std::size_t byte_send) {
            
            if (!error) {
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}

std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> NetworkManager::getLastMsg()
{
    std::lock_guard<std::mutex> lock(mtx);
    if (messages.empty())
        return {};
    auto msg = messages.front();
    messages.pop();
    return msg;
}


asio::ip::udp::endpoint NetworkManager::getLastSender() const
{
    return last_sender_;
}
