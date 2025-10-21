/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_server
*/

/**
 * @file init_server.cpp
 * @author Farouk OKANLA
 * @brief This file contains the definition of the server initializing funcions
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c)
 *
 */

#include "../include/server.hpp"

/**
 * @brief This function uses the ResourceManager to pre-load textures and fonts
 * that will be used in the game
 *
 */
void load_textures(void) {
    ResourceManager::Instance().load(
        "assets/sprites/player/player1.gif", "player1", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player2.gif", "player2", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_up.gif", "player_up", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_down.gif", "player_down", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_missile.gif", "player_missile", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/red_trooper.gif", "red_trooper", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/walker_walk.gif", "walker", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/enemy_missile.gif", "enemy_missile", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/effects/Explosion.png", "explosion", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/effects/hit_effect.gif", "hit_effect", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/background/background.jpg", "background", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/background/wall1_shadow.png", "ceiling", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss.gif", "boss", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT
    );
}

void Server::initializeGame(void)
{
    factory.make_background();
    player1_entity_id = factory.make_entity("player1");
    player2_entity_id = factory.make_entity("player2");
    factory.make_ceiling();
    factory.make_floor();

    auto& pos1 =
        reg.get_components<component::position>()[player1_entity_id].value();
    auto& pos2 =
        reg.get_components<component::position>()[player2_entity_id].value();
    pos1.x = 50;
    pos1.y = 250;

    pos2.x = 50;
    pos2.y = 150;
    // factory.make_menu_background_music();
}

Server::Server(int p) :
    win(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type Server"),
    reg(win), factory(reg), server_(p, std::ref(messages), std::ref(mtx)), p_(p)
{
    reg.control_active = false;
    counter = 0;

    load_textures();
    initializeGame();
    if (diff_mode == EASY)
        loadLevel("assets/levels/easy.txt");
    else if (diff_mode == HARD)
        loadLevel("assets/levels/hard.txt");
    else
        loadLevel("assets/levels/medium.txt");
}

/**
 * @brief Destroy the Server:: Server object
 *
 */
Server::~Server() {}

void Server::run()
{
    win.setFramerateLimit(60);
    networkThread = std::thread([this]() { server_.run(); });

    while (win.isOpen()) {
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }

        double dt = frameClock.restart().asSeconds();

        reg.run_systems(dt);

        logGameEntities();

        runLevel(dt);

        if (player1_entity_id == -1 && player2_entity_id == -1) {
            printf("GAME OVER\n");
            break;
        }
        if (boss_dead) {
            printf("BOSS DEAD\n");
            break;
        }
    }

    server_.stop();
    networkThread.join();
}
