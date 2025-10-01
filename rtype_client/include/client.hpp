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

typedef enum {
    MENU,
    TRANSITION,
    GAME,
    GAME_OVER
}state_t;

typedef struct menu_info_s {
    entity background;
    entity title;
    entity start_text;
    entity menu_background_music;
    entity menu_fade_in_rect;
    entity menu_fade_out_rect;    
} menu_info_t;

class Client {
  private:
    registry &_reg;
    int port_;
    double levelTimer = 0.0;
    std::vector<entity> active_entities;
    MoveRequest move;
    MoveResponse check;
    NetworkManager client_;

    menu_info_t menu_info;
    
    public:
    state_t state = MENU;
    Client(int p, std::string a, registry &reg);
    MoveRequest getMoveKey();
    ~Client();

    void initMenu();
    void runMenu(double delta);
    void initGame();
    void runLevel(double delta);
};


#endif /* !CLIENT_HPP_ */
