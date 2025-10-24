/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

/**
 * @file server.hpp
 * @author Farouk OKANLA
 * @brief The file contains the definition of the server class
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <algorithm>
#include <vector>

#include "Factory.hpp"
#include "Network.hpp"
#include "registry.hpp"

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

/**
 * @brief A global variable to store the id of the first player in the registry
 *
 */
inline int player1_entity_id = -1;
/**
 * @brief A global variable to store the id of the second player in the registry
 *
 */
inline int player2_entity_id = -1;

/**
 * @brief An enum to store the difficulty of the game
 *
 */
typedef enum diff_mode { EASY, MEDIUM, HARD } diff_mode_t;

/**
 * @brief A gloal variable to store the difficulty of the game
 *
 */
inline diff_mode_t diff_mode = MEDIUM;

/**
 * @brief A struct to store the information of an entity to be spawned on the
 * level
 *
 */
typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

typedef enum { LEVEL1, LEVEL2, LEVEL3 } state_t;

/**
 * @brief The server class. Handles the server side of the game.
 * At the start of the game, the server loads the level entities from a
 * configuration file The server holds the simulation of the game with all these
 * entities and teh players. It listens from inputs from the clients and sends
 * the game state to the them on each frame
 */
class Server {
  private:
    int p_;
    registry& reg;
    double levelTimer = 0.0;
    std::mutex mtx;

    void loadLevel();
    void clearGameEntities();
    void initializeGame(void);
    void initializePlayers(void);
    void logGameEntities(void);
    void receivePlayerInput(double delta);

    NetworkManager server_;

    std::vector<entity_info_t> entities;

    std::vector<int8_t> result;

    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>
        messages;

    int counter = 0;

    std::map<asio::ip::udp::endpoint, int> all_clients;

  public:
    Server(int p, registry& reg);
    ~Server();

    NetworkManager& getManager() {
        return server_;
    }

    // encodeur
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);
    std::vector<int8_t> encodeGameState(const GameState& pos);

    // decodeur
    MoveResponse decodeMoveResponse(std::vector<int8_t>& buffer);
    ActionResponse decodeActionResponse(std::vector<int8_t>& buffer);

    void runLevel(double delta);
    void handleWinOrLoss();

    state_t state = LEVEL1;
};

#endif /* !SERVER_HPP_ */
