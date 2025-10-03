/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

#include "../include/server.hpp"

void load_textures(void)
{
    ResourceManager::Instance().load("assets/sprites/player/player.gif", "player", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_up.gif", "player_up", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_down.gif", "player_down", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player_missile.gif", "player_missile", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/red_trooper.gif", "red_trooper", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/walker_walk.gif", "walker", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/enemies/enemy_missile.gif", "enemy_missile", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/effects/Explosion.png", "explosion", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/effects/hit_effect.gif", "hit_effect", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/background/background.jpg", "background", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/background/wall1_shadow.png", "ceiling", TEXTURE);

    ResourceManager::Instance().load("assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT);
}

void Server::initializeGame(void)
{
    Factory factory(reg);
    
    factory.make_background();
    player_entity_id = factory.make_entity("player");
    
    auto &pos = reg.get_components<component::position>()[player_entity_id].value();
    pos.x = 50;
    pos.y = 50;

    factory.make_menu_background_music();
}

Server::Server(int p, registry &regis) : server_(8080, "127.0.0.1"), p_(p), reg(regis)
{
    response = {};
    response.type = 0x24;
    response.player_id = 1;
    response.position = {100, 200};
    response.direction = {0, 0};
    response.speed = 0;
    response.timestamp.milliseconds = 0;

    load_textures();
    initializeGame();
    loadLevel("assets/levels/test.txt");
    // while (1) {
    //     server_.poll();

    //     auto msg = server_.getLastMsg();

    //     if (!msg.first.empty()) {
    //         u_int8_t type = msg.first[0];
    //         auto sender = msg.second;

    //         if (type == 0x23) {
    //             MoveRequest req = decodeMoveRequest(msg.first);
    //             std::cout << "Client direction = " << static_cast<int>(req.direction) << std::endl;

    //             MoveResponse newResp = response;
    //             newResp.type = 0x24;
    //             newResp.player_id = 1;
    //             newResp.speed = 2;
    //             newResp.timestamp.milliseconds =
    //                 std::chrono::duration_cast<std::chrono::milliseconds>(
    //                     std::chrono::system_clock::now().time_since_epoch()).count();

    //             switch (req.direction) {
    //                 case UP:
    //                     newResp.direction = {0, static_cast<uint16_t>(-1)};
    //                     break;
    //                 case DOWN:
    //                     newResp.direction = {0, 1};
    //                     break;
    //                 case LEFT:
    //                     newResp.direction = {static_cast<uint16_t>(-1), 0};
    //                     break;
    //                 case RIGHT:
    //                     newResp.direction = {1, 0};
    //                     break;
    //             }

    //             response = updateMoveResponse(response, newResp);


    //             std::vector<uint8_t> buff = encodeMoveResponse(response);
    //             server_.send(buff, buff.size(), sender);
    //         }
    //     }
    // }
}

MoveResponse Server::getMove()
{
    return response;
}

Server::~Server()
{

}
