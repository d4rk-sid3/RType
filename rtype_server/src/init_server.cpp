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

Server::Server(int p, registry &regis) : server_(8080), p_(p), reg(regis)
{
    load_textures();
    initializeGame();
    loadLevel("assets/levels/test.txt");

    // std::thread input_thread([this, ]() {
       server_.poll();
    // });

    std::thread send_thread([this]() {
        while (true) {
            std::lock_guard<std::mutex> lock(mtx);

            if (result.size() == 0){
                std:: cout << "No data to send" << std::endl;
                continue;
            }
            server_.send(result, result.size(), server_.getLastSender());
        }
    });

    send_thread.detach();
}

Server::~Server()
{

}


