/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "Network.hpp"
#include <vector>
#include "registry.hpp"

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
    Factory factory;
    double levelTimer = 0.0;
    

    void loadLevel(std::string &path);
    void Server::runLevel(double delta);

  public:
    Server(int p);
    ~Server();

    std::vector<entity_info_t> entities;

};

#endif /* !SERVER_HPP_ */
