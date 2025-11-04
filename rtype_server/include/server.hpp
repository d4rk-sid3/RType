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
#include "logic_functions.hpp"
#include "Types.hpp"

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
 * @brief A global variable to store the id of the third player in the registry
 *
 */
inline int player3_entity_id = -1;
/**
 * @brief A global variable to store the id of the fourth player in the registry
 *
 */
inline int player4_entity_id = -1;

/**
 * @brief Map of all connected clients and their player IDs
 */
inline std::map<asio::ip::udp::endpoint, int> all_clients;

/**
 * @brief An enum to store the difficulty of the game
 *
 */
typedef enum diff_mode {EASY, MEDIUM, HARD, PVP, CUSTOM } diff_mode_t;

/**
 * @brief A gloal variable to store the difficulty of the game
 *
 */
inline diff_mode_t diff_mode = MEDIUM;

/**
 * @brief A global variable that holds the path to the custom conf file
 *
 */
inline std::string custom_conf_path = "";

typedef enum { LEVEL1, LEVEL2, LEVEL3, CUSTOM_LEVEL } state_t;


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
     * @brief The Duration of one tick
     */
    const chrono::milliseconds tickDuration = std::chrono::milliseconds(50);

    /**
     * @brief The window for rendering (if needed)
     */
    sf::RenderWindow win;

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
     * @brief The network manager that handles communication with clients
     */
    NetworkManager server_;

    /**
     * @brief Thread for running the network manager
     */
    std::thread networkThread;

    /**
     * @brief List of all entities and their information in the game
     */
    std::vector<entity_info_t> entities;

    /**
     * @brief Buffer for storing outgoing messages
     */
    std::vector<int8_t> result;

    /**
     * @brief Mutex for thread safety with the network manager
     */
    std::mutex mtx;

    /**
     * @brief List of all last messages received from clients
     */
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>>
        messages;

    /**
     * @brief Event for handling window events
     */
    sf::Event event;

    /**
     * @brief Clock for managing frame time
     */
    sf::Clock frameClock;

    /**
     * @brief The current level timer
     */
    double levelTimer = 0.0;

    /**
    * @brief Counter used to know how many entities have been sent to the clients
    */
    int counter;

    /**
     * @brief The starting moment of the game
     */
    std::chrono::time_point<std::chrono::steady_clock> gameStarted;

    /**
    * @brief Counter used to know how many packages have been sent to the clients
    */
    uint32_t packageId;

    /**
     * @brief Load all information about the level from a file
     * @param path The path to the level file
     */
    void loadLevel();

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

    void clearGameEntities();

    void initializePlayers(void);

    void initializePlayersPVP(void);

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
     * @param pos The MessageHeader structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeMessageHeader(const MessageHeader& pos);

    /**
     * @brief Encode an EnemyMovedResponse structure into a byte buffer
     * @param pos The EnemyMovedResponse structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);

    /**
     * @brief Encode an GameState structure into a byte buffer
     * @param pos The GameState structure to encode
     * @return A vector of int8_t representing the encoded data
     */
    std::vector<int8_t> encodeGameState(const GameState& pos);

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

    void handleWinOrLoss();

    state_t state = LEVEL1;
};

#endif /* !SERVER_HPP_ */
