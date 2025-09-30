/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include "../../utility_classes/include/Network.hpp"
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

class Client {
  private:
    registry &_reg;
    int port_;
    MoveRequest move;
    MoveResponse check;
    NetworkManager client_;
  
    double levelTimer = 0.0;

    void spawn_player(void);

    std::vector<entity> active_entities;


  public:
    Client(int p, std::string a, registry &reg);
    MoveRequest getMoveKey();
    ~Client();

    void runLevel(double delta);
};


#endif /* !CLIENT_HPP_ */
