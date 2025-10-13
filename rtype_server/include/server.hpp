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

#include "Factory.hpp"
#include "Network.hpp"
#include "registry.hpp"
#include <algorithm>
#include <vector>

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
 * @brief A struct to store the information of an entity to be spawned on the level
 * 
 */
typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

/**
 * @brief The server class. Handles the server side of the game.
 * At the start of the game, the server loads the level entities from a configuration file
 * The server holds the simulation of the game with all these entities and teh players.
 * It listens from inputs from the clients and sends the game state to the them on each frame
 */
class Server {
  private:
    int p_;
    registry &reg;
    double levelTimer = 0.0;
    std::mutex mtx;
  
    void loadLevel(const std::string &path);
    void initializeGame(void);
    void logGameEntities(void);
    void receivePlayerInput(double delta);

    NetworkManager server_;

    std::vector<entity_info_t> entities;

    std::vector<int8_t> result;
  
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> messages;

    int counter = 0;

    std::map<asio::ip::udp::endpoint, int> all_clients;

  public:
    Server(int p, registry &reg);
    ~Server();

    NetworkManager &getManager() { return server_; }

    // encodeur
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);

    // decodeur
    MoveResponse decodeMoveResponse(std::vector<int8_t>& buffer);
    ActionResponse decodeActionResponse(std::vector<int8_t>& buffer);

    void runLevel(double delta);
};

#endif /* !SERVER_HPP_ */
