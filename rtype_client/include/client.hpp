/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include <algorithm>
#include <vector>

#include "../../utility_classes/include/Network.hpp"
#include "Factory.hpp"
#include "registry.hpp"
#include "Factory.hpp"
#include <map>

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline int player_entity_id = 0;

typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

typedef enum { MENU, TRANSITION, GAME, GAME_OVER } state_t;

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
    registry& _reg;
    int port_;
    menu_info_t menu_info;
    double levelTimer = 0.0;
    NetworkManager client_;
    std::mutex mtx;

    // This map associates the servers_ids to the client_ids in the registry
    std::unordered_map<size_t, size_t> ids_assoc;
    std::vector<int8_t> lastmsg;

    // std::vector<...> old;
    // std::vector<...> new;

    public:
    state_t state = GAME;
    Client(int p, std::string a, registry &reg);
    ~Client();

    void initMenu();
    void runMenu(double delta);
    void initGame();
    void runLevel(double delta);
    void sendPlayerInput();

    NetworkManager &getManager() { return client_; }

    std::vector<EnemyMovedResponse> recupAllEntities();
    
    // decodeur
    NbrEntity decodeNbrEntity(std::vector<int8_t>& buffer);
    EnemyMovedResponse decodeEnemyMovedResponse(std::vector<int8_t>& buffer);
    std::vector<int8_t> encodeMoveResponse(const MoveResponse& pos);
};

#endif /* !CLIENT_HPP_ */
