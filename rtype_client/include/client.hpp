/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

/**
 * @file client.hpp
 * @author your name (you@domain.com)
 * @brief The client class definition file
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <algorithm>
#include <map>
#include <vector>

#include "../../utility_classes/include/Network.hpp"
#include "Factory.hpp"
#include "entity.hpp"
#include "registry.hpp"
#include "UIHandler.hpp"
#include "logic_functions.hpp"

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline bool player1_dead = false;
inline bool player2_dead = false;

inline int controllable_id = -1;

/**
 * @brief A struct to store infos on an entity to be spawned in the level
 *
 */
typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

/**
 * @brief An enum to define the different states of the game
 *
 */
typedef enum { MENU, TRANSITION, LEVEL1, LEVEL2, LEVEL3 } state_t;
typedef enum { GAME_OVER, VICTORY, LEVEL1_START, LEVEL2_START, LEVEL3_START, SUB_NONE } substate_t;

typedef struct menu_info_s {
    entity background;
    entity title;
    entity start_text;
    entity menu_background_music;
    entity menu_fade_in_rect;
    entity menu_fade_out_rect;
} menu_info_t;

/**
 * @brief The client class. Handles the client side of the game.
 * All the client does is send messages to the server if the player tries to
 * move, and also receive the game state from the server in order to update the
 * game
 *
 */
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
    
    std::vector<EnemyMovedResponse> old;
    std::vector<EnemyMovedResponse> new_vec;

    public:
    /**
     * @brief The current state of the game
     *
     */
    UIHandler ui_handler;
    state_t state = LEVEL1;
    substate_t substate = LEVEL1_START;

    Client(int p, std::string a, registry& reg);
    ~Client();

    void initMenu();
    void runMenu(double delta);
    void initGame();
    void runLevel(double delta);
    void sendPlayerInput();
    void sendPlayerAction();
    void handleSubStates(double delta, sf::RenderWindow& win);

    NetworkManager& getManager() {
        return client_;
    }

    std::vector<EnemyMovedResponse> recupAllEntities();

    // decodeur
    NbrEntity decodeNbrEntity(std::vector<int8_t>& buffer);
    GameState decodeGameState(std::vector<int8_t>& buffer);
    EnemyMovedResponse decodeEnemyMovedResponse(std::vector<int8_t>& buffer);
    std::vector<int8_t> encodeMoveResponse(const MoveResponse& pos);
    std::vector<int8_t> encodeActionResponse(const ActionResponse& pos);
};

#endif /* !CLIENT_HPP_ */
