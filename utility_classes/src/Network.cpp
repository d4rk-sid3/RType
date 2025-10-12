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
    {"ceiling", TYPE_10},
    {"boss", TYPE_11}
};


NetworkManager::NetworkManager(int port, std::string address, std::vector<int8_t> &lastmsg_, std::mutex& mtx_): 
    socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
    server_endpoint_(asio::ip::udp::endpoint(asio::ip::make_address(address), port)),
    isrunning(true), lastmsg(lastmsg_), mtx(mtx_), clients_lastmsg(tmp_clients)
{
    std::cout << "Client lancé " << std::endl;

    receive_from_server();
}


NetworkManager::NetworkManager(int port, std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> &clients_lastmsg_, std::mutex& mtx_) :
    socket(context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
    isrunning(true), clients_lastmsg(clients_lastmsg_), mtx(mtx_), lastmsg(tmp_server)
{
    std::cout << "Serveur pret à être lancé " << port << std::endl;

    receive_from_clients();
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

void NetworkManager::receive_from_clients()
{
    socket.async_receive_from(asio::buffer(buff), last_sender_, 
        
        [this](std::error_code error ,std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                std::cout << "Receive :" << bytes_receive << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    clients_lastmsg.push_back({last_sender_, std::vector<int8_t>(buff.begin(), buff.begin() + bytes_receive)});
                }
            }
            if (isrunning) {
                receive_from_clients();
            }
        }
    );
}

void NetworkManager::send_to_client(const std::vector<int8_t> &msg, size_t size, const asio::ip::udp::endpoint& to_client)
{
    socket.async_send_to(asio::buffer(msg, size * sizeof(int8_t)), to_client,
        [msg](std::error_code error, std::size_t byte_send) {
            std::cout << "Send :" << byte_send << std::endl;

            if (!error) {
            } else {
                std::cerr << error.message() << std::endl;
            }
        }
    );
}

void NetworkManager::receive_from_server()
{
    socket.async_receive_from(asio::buffer(buff), last_sender_, 
        
        [this](std::error_code error ,std::size_t bytes_receive) {
            if (!error && bytes_receive > 0) {
                std::cout << "Receive :" << bytes_receive << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    lastmsg.insert(lastmsg.end(), buff.begin(), buff.begin() + bytes_receive);
                }
            }

            if (isrunning) {
                receive_from_server();
            }
        }
    );
}

void NetworkManager::send_to_server(const std::vector<int8_t> &msg, size_t size)
{
    socket.async_send_to(asio::buffer(msg, size * sizeof(int8_t)), server_endpoint_,
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

asio::ip::udp::endpoint& NetworkManager::getServerendpoint()
{
    return server_endpoint_;
}

asio::io_context& NetworkManager::getContext()
{
    return context;
}
