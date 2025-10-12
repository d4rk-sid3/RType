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
    int p_;
    registry reg;
    Factory factory;
    double levelTimer = 0.0;
    std::mutex mtx;
    std::mutex regMtx;
    NetworkManager server_;
    std::vector<entity_info_t> entities;
    std::vector<int8_t> result;
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<int8_t>>> messages;
    std::map<asio::ip::udp::endpoint, int> all_clients;
    sf::RenderWindow win;
    sf::Event event;
    sf::Clock frameClock;
    int counter;
    std::atomic<bool> isRunning;


    void loadLevel(const std::string &path);
    void initializeGame(void);
    void logGameEntities(void);
    void receivePlayerInput(double delta);
    void runLevel(double delta);

    std::thread networkThread;
    std::thread cooldownThread;

  public:

    Server(int p);
    ~Server();

    NetworkManager &getManager() { return server_; }

    // encodeur
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos);

    // decodeur
    MoveResponse decodeMoveResponse(std::vector<int8_t>& buffer);
    ActionResponse decodeActionResponse(std::vector<int8_t>& buffer);

    void run();
};

#endif /* !SERVER_HPP_ */
