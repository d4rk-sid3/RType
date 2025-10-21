/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

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

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <algorithm>
#include <vector>

#include "Factory.hpp"
#include "Network.hpp"
#include "registry.hpp"
#include "logic_functions.hpp"

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

/**
 * @brief The server class. Handles the server side of the game.
 * At the start of the game, the server loads the level entities from a
 * configuration file The server holds the simulation of the game with all these
 * entities and teh players. It listens from inputs from the clients and sends
 * the game state to the them on each frame
 */
class Server {
  private:
    /**
     * @brief The port to listen on
     */
    int p_;

    /**
     * @brief The registry that holds all entities and components
     */
    registry reg;

    /**
     * @brief The factory that creates entities and components
     */
    Factory factory;

    /**
     * @brief The current level timer
     */
    double levelTimer = 0.0;

    /**
     * @brief Mutex for thread safety with the network manager
     */
    std::mutex mtx;

    void loadLevel(const std::string& path);
    void initializeGame(void);
    void logGameEntities(void);
    void receivePlayerInput(double delta);

    /**
     * @brief The network manager that handles communication with clients
     */
    NetworkManager server_;

    /**
     * @brief List of all entities and their information in the game
     */
    std::vector<entity_info_t> entities;

    /**
     * @brief Buffer for storing outgoing messages
     */
    std::vector<int8_t> result;

    /**
     * @brief List of all last messages received from clients
     */
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>
        messages;

    /**
     * @brief Map of all connected clients and their player IDs
     */
    std::map<asio::ip::udp::endpoint, int> all_clients;

    /**
     * @brief The window for rendering (if needed)
     */
    sf::RenderWindow win;

    /**
     * @brief Event for handling window events
     */
    sf::Event event;

    /**
     * @brief Clock for managing frame time
     */
    sf::Clock frameClock;

    /**
    * @brief Counter used to know how many entities have been sent to the clients
    */
    int counter;

    /**
     * @brief Thread for running the network manager
     */
    std::thread networkThread;

    /**
     * @brief Load all information about the level from a file
     * @param path The path to the level file
     */
    void loadLevel(const std::string &path);

    /**
     * @brief Initialize all game related elements
     */
    void initializeGame(void);

    /**
     * @brief Log all game entities and send their states to clients
     */
    void logGameEntities(void);

    /**
     * @brief Receive and process player input from clients
     * @param delta The time elapsed since the last frame
     */
    void receivePlayerInput(double delta);

    /**
     * @brief Manage the game level, including spawning entities and processing game logic
     * @param delta The time elapsed since the last frame
     */
    void runLevel(double delta);

  public:

    /**
     * @brief Construct a new Server object
     * @param p The port to listen on
     */
    Server(int p);

    /**
     * @brief Destroy the Server object
     */
    ~Server();

    /**
     * @brief Encode the number of entities into a byte buffer
     * @param pos The NbrEntity structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);

    /**
     * @brief Encode an EnemyMovedResponse structure into a byte buffer
     * @param pos The EnemyMovedResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);

    /**
     * @brief Decode a MoveResponse structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded MoveResponse structure
     */
    MoveResponse decodeMoveResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Decode an ActionResponse structure from a byte buffer
     * @param buffer The byte buffer containing the encoded data
     * @return The decoded ActionResponse structure
     */
    ActionResponse decodeActionResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Run the server application
     */
    void run();
};

#endif /* !SERVER_HPP_ */
