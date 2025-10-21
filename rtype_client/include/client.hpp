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
#include <algorithm>
#include <vector>
#include <map>

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

/**
 * @brief A global variable to store the id of the player in the registry
 * This variable is useful to access infos on the player all accross the program
 *
 */
inline int player_entity_id = -1;

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
typedef enum { MENU, TRANSITION, GAME, GAME_OVER } state_t;

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

    /**
     * @brief Window for rendering
     */
    sf::RenderWindow win;

    /**
     * @brief The registry that holds all entities and components
     */
    registry reg;

    /**
     * @brief The factory that creates entities and components
     */
    Factory factory;

    /**
     * @brief The port to connect to the server
     */
    int port_;

    /**
     * @brief The menu information
     */
    menu_info_t menu_info;

    /**
     * @brief The timer for the level
     */
    double levelTimer = 0.0;

    /**
     * @brief The network manager that handles communication with the server
     */
    NetworkManager client_;

    /**
     * @brief Mutex for thread safety
     */
    std::mutex mtx;

    /**
     * @brief Map that associates server entity IDs to client entity IDs
     */
    std::unordered_map<size_t, size_t> ids_assoc;

    /**
     * @brief The last message received from the server
     */
    std::vector<int8_t> lastmsg;

    /**
     * @brief Vectors to hold old and new entity states for comparison
     */
    std::vector<EnemyMovedResponse> old;
    std::vector<EnemyMovedResponse> new_vec;

    std::vector<std::vector<EnemyMovedResponse>> entity_states;

    /**
     * @brief Event for handling window events
     */
    sf::Event event;

    /**
     * @brief The current state of the game (menu, transition, game, game over)
     */
    state_t state;

    /**
     * @brief Clock for managing frame time
     */
    sf::Clock frameClock;

    /**
     * @brief Thread for running the network manager
     */
    std::thread networkThread;

    /**
     * @brief Initialize all menu related elements
     */
    void initMenu();

    /**
     * @brief Run the menu logic
     * @param delta The time elapsed since the last frame
     */
    void runMenu(double delta);

    /**
     * @brief Initialize all game related elements
     */
    void initGame();

    /**
     * @brief Run the game logic
     * @param delta The time elapsed since the last frame
     */
    void runLevel(double delta);

    /**
     * @brief Send the player's input to the server
     */
    void sendPlayerInput();

    /**
     * @brief Send the player's action (e.g., shooting) to the server
     */
    void sendPlayerAction();

  public:
    /**
     * @brief Construct a new Client object
     * @param p The port to connect to the server
     * @param addr The address of the server
     */
    Client(int p, std::string addr);

    /**
     * @brief Destroy the Client object
     */
    ~Client();

    /**
     * @brief Get all entities from the last server message
     * @return A vector of EnemyMovedResponse structures representing the entities
     */
    std::vector<EnemyMovedResponse> recupAllEntities();

    /**
     * @brief Decode a NbrEntity structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded NbrEntity structure
     */
    NbrEntity decodeNbrEntity(std::vector<int8_t>& buffer);

    /**
     * @brief Decode an EnemyMovedResponse structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded EnemyMovedResponse structure
     */
    EnemyMovedResponse decodeEnemyMovedResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Encode a MoveResponse structure into a byte buffer
     * @param pos The MoveResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeMoveResponse(const MoveResponse& pos);

    /**
     * @brief Encode an ActionResponse structure into a byte buffer
     * @param pos The ActionResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeActionResponse(const ActionResponse& pos);

    /**
     * @brief Run the client application
     */
    void run();
};

#endif /* !CLIENT_HPP_ */

