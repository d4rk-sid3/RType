/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include "../include/Network.hpp"

std::map<std::string, EnemyType> type_map = {
    {"player1", TYPE_1},
    {"player2", TYPE_2},
    {"red_trooper", TYPE_3},
    {"walker", TYPE_4},
    {"player_missile", TYPE_5},
    {"enemy_missile", TYPE_6},
    {"explosion", TYPE_7},
    {"hit_effect", TYPE_8},
    {"floor", TYPE_9},
    {"ceiling", TYPE_10}
};

NetworkManager::NetworkManager(int port, std::string address, std::vector<int8_t> &lastmsg_, std::mutex& mtx_): 
    socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
    server_endpoint_(asio::ip::udp::endpoint(asio::ip::make_address(address), port)),
    isrunning(true), lastmsg(lastmsg_), mtx(mtx_)
{
    std::cout << "Client lancé " << std::endl;

    receive();
}

NetworkManager::NetworkManager(int port, std::vector<int8_t> &lastmsg_, std::mutex& mtx_) :
    socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
    isrunning(true), lastmsg(lastmsg_), mtx(mtx_)
{
    std::cout << "Serveur pret à être lancé " << port << std::endl;

    receive();
}


NetworkManager::~NetworkManager()
{
    isrunning = false;
    socket.close();
}

void NetworkManager::poll()
{
    if (isrunning) {
        context.poll();
    }
}

void NetworkManager::run()
{
    if (isrunning) {
        context.run();
    }
}

void NetworkManager::add_connection(const asio::ip::udp::endpoint& ep) {
    if (std::find(clients.begin(), clients.end(), ep) == clients.end())
        clients.push_back(ep);
}

void NetworkManager::receive()
{
    socket.async_receive_from(asio::buffer(buff), last_sender_, 
        
        [this](std::error_code error ,std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                add_connection(last_sender_);
                std::cout << "Receive :" << bytes_receive << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    for (auto &b : buff) {
                        lastmsg.push_back(b);
                    }
                }
            }

            if (isrunning) {
                receive();
            }
        }
    );
}

void NetworkManager::send(const std::vector<int8_t> &msg, size_t size, const asio::ip::udp::endpoint& to_receiver)
{
    socket.async_send_to(asio::buffer(msg, size * sizeof(int8_t)), to_receiver,
        [msg](std::error_code error, std::size_t byte_send) {
            std::cout << "Send :" << byte_send << std::endl;

            if (!error) {
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}

asio::ip::udp::endpoint NetworkManager::getLastSender() const
{
    return last_sender_;
}
