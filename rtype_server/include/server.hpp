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
    registry &reg;
    double levelTimer = 0.0;
    std::mutex mtx;
  
    void loadLevel(const std::string &path);
    void initializeGame(void);
    void logGameEntities(void);
    
    NetworkManager server_;

    std::vector<entity_info_t> entities;

    vector<int8_t> result;
    vector<int8_t> tmp;
    int counter = 0;

  public:
    void runLevel(double delta);
    Server(int p, registry &reg);
    ~Server();

    NetworkManager &getManager() { return server_; }

    // encodeur
    std::vector<int8_t> encodeNbrEntity(const NbrEntity& pos);
    std::vector<int8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos
    );

};

#endif /* !SERVER_HPP_ */
