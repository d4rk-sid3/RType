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

    ResourceManager::Instance().load("assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT);
}

void Server::spawn_player(void)
{
    factory.make_background();
    player_entity_id = (int)factory.make_entity("player");
    active_entities.push_back((entity)player_entity_id);
    
    printf("Player init\n");
    auto &pos = reg.get_components<component::position>()[player_entity_id].value();
    pos.x = 50;
    pos.y = 50;

    // factory.make_title();
    // factory.make_start_text();
    factory.make_menu_background_music();
}

Server::Server(int p, registry &regis, Factory &fac) : server_(8080, "127.0.0.1"), p_(p), reg(regis), factory(fac)
{
    std::vector<asio::ip::udp::endpoint> client_endpoint;

    load_textures();
    spawn_player();
    loadLevel("assets/levels/test.txt");
    // while (1) {
    //     server_.poll();

    //     auto msg = server_.getLastMsg();

    //     if (!msg.first.empty()) {
    //         u_int8_t type = msg.first[0];
    //         auto sender = msg.second;

    //         if (type == 0x23 && msg.first.size() >= sizeof(MoveRequest)) {
    //             MoveRequest res{};
    //             MoveResponse resp = recupMove(msg, res);
    //             server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
    //         } else if (type == 0x27) {
    //             PickupItemResponse resp = recupItem(msg);
    //             server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
    //         } else if (type == 0x28) {
    //             PlayerStateResponse resp = player_r(msg);
    //             server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
    //         } else if (type == 0x29) {
    //             PlayerStateResponse2 resp = player_r2(msg);
    //             server_.send(reinterpret_cast<uint8_t*>(&resp), sizeof(resp), sender);
    //         } else {
    //             std::cerr << "Message inconnue: " << (int)type
    //                 << "size= " <<  msg.first.size() << " bytes" << std::endl;
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
