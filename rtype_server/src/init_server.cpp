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
        "assets/sprites/player/player2_flipped.gif", "player2_flipped", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/player/player_missile_flipped.gif", "player_missile_flipped", TEXTURE
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
        "assets/sprites/enemies/plane.gif", "plane", TEXTURE
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
        "assets/sprites/player/force.gif", "force", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/enemy_jet.gif", "green_trooper", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/tourelles.gif", "tourelles", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss_big_shooter.gif", "big_shooter", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/boss_small_shooter.gif", "small_shooter", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/final_boss.gif", "final_boss", TEXTURE
    );
    ResourceManager::Instance().load(
        "assets/sprites/enemies/big_missile.gif", "big_missile", TEXTURE
    );

    ResourceManager::Instance().load(
        "assets/sprites/enemies/space_enemies.gif", "space_enemy", TEXTURE
    );
}

void Server::initializeGame(void) {
    factory.make_background();
    factory.make_ceiling();
    factory.make_floor();
    factory.make_menu_background_music();
}

/**
 * @brief This function initializes the players
 *
 */
void Server::initializePlayers(void) {
    if (player1_entity_id == -1) 
        player1_entity_id = factory.make_entity("player1");
    if (player2_entity_id == -1)
        player2_entity_id = factory.make_entity("player2");

    auto& pos1 =
        reg.get_components<component::position>()[player1_entity_id].value();
    auto& pos2 =
        reg.get_components<component::position>()[player2_entity_id].value();
    pos1.x = 50;
    pos1.y = 250;

    pos2.x = 50;
    pos2.y = 150;
}

/**
 * @brief This function initializes the players for pvp mode
 *
 */
void Server::initializePlayersPVP(void) {
    if (player1_entity_id == -1) 
        player1_entity_id = factory.make_entity("player1");
    if (player2_entity_id == -1)
        player2_entity_id = factory.make_entity("player2_flipped");

    auto& pos1 =
        reg.get_components<component::position>()[player1_entity_id].value();
    auto& pos2 =
        reg.get_components<component::position>()[player2_entity_id].value();
    pos1.x = 50;
    pos1.y = 250;

    pos2.x = 50;
    pos2.y = 250;
}

Server::Server(int p) :
    p_(p),
    reg(), factory(reg), server_(p, std::ref(messages), std::ref(mtx))
{
    reg.toggleLogic();
    reg.toggleMovement();
    reg.togglePhysics();
    counter = 0;

    load_textures();
    initializeGame();
    if (custom_conf_path != "")
        state = CUSTOM_LEVEL;
    loadLevel();
}

/**
 * @brief Destroy the Server:: Server object
 *
 */
Server::~Server() {}

void Server::run()
{
    networkThread = std::thread([this]() { server_.run(); });

    gameStarted = std::chrono::steady_clock::now();

    while (1) {
        auto start = std::chrono::steady_clock::now();

        double dt = frameClock.restart().asSeconds();

        reg.run_systems(dt);

        logGameEntities();

        runLevel(dt);

        handleWinOrLoss();

        std::this_thread::sleep_until(start + tickDuration);
    }

    server_.stop();
    networkThread.join();
}
