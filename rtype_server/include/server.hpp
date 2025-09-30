/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "Network.hpp"
#include <vector>
#include <algorithm>
#include "registry.hpp"
#include "Factory.hpp"

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline int player_entity_id = 0;

typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

class Server {
  private:
    int p_;
    registry &reg;
    Factory &factory;
    double levelTimer = 0.0;
  
    void loadLevel(const std::string &path);
    
    void spawn_player(void);
    
    MoveResponse response;
    MoveRequest move;
    NetworkManager server_;
    PickupItemResponse item;
    PlayerStateResponse p_response;
    PlayerStateResponse2 p_response2;

    public:
    void runLevel(double delta);
    Server(int p, registry &reg, Factory &fac);
    MoveResponse getMove();
    MoveResponse recupMove(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a, MoveRequest &m);

    PickupItemResponse recupItem(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);
    PlayerStateResponse player_r(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);
    PlayerStateResponse2 player_r2(const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> &a);

    ~Server();

    std::vector<entity_info_t> entities;

    std::vector<entity> active_entities;
};

#endif /* !SERVER_HPP_ */
