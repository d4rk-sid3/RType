/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
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

inline int player1_entity_id = -1;
inline int player2_entity_id = -1;

typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

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

    /**
     * @brief Mutex for thread safety with the registry
     */
    std::mutex regMtx;

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
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> messages;

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
     * @brief Flag to control the running state of the server
     */
    std::atomic<bool> isRunning;


    /**
     * @brief Load all information about the level from a file
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
     */
    void receivePlayerInput(double delta);

    /**
     * @brief Manage the game level, including spawning entities and processing game logic
     */
    void runLevel(double delta);

    /**
     * @brief Thread for running the network manager
     */
    std::thread networkThread;

    /**
     * @brief Thread for managing information sending cooldowns to clients
     */
    std::thread cooldownThread;

  public:

    /**
     * @brief Construct a new Server object
     */
    Server(int p);

    /**
     * @brief Destroy the Server object
     */
    ~Server();

    NetworkManager &getManager() { return server_; }

    // encodeur
    /**
     * @brief Encode the number of entities into a byte buffer
     */
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);

    /**
     * @brief Encode an EnemyMovedResponse structure into a byte buffer
     */
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);

    // decodeur
    /**
     * @brief Decode a MoveResponse structure from a byte buffer
     */
    MoveResponse decodeMoveResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Decode an ActionResponse structure from a byte buffer
     */
    ActionResponse decodeActionResponse(std::vector<int8_t>& buffer);

    /**
     * @brief Run the server application
     */
    void run();
};

#endif /* !SERVER_HPP_ */
