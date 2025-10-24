/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

/**
 * @file init_client.cpp
 * @author Farouk OKANLA
 * @brief This file contains the definition of the client functions
 * @version 0.1
 * @date 2025-10-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../include/client.hpp"
#include "components.hpp"

/**
 * @brief This function uses the ResourceManager to pre-load textures
 * and fonts that will be used in the game
 */
void load_client_textures(void) {
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

/**
 * @brief Construct a new Client:: Client object
 *
 * @param p The port to connect to
 * @param address The server address
 */
Client::Client(int p, std::string address):
    port_(p), client_(p, address, std::ref(lastmsg), std::ref(mtx)),
    win(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "R-Type"), reg(win), factory(reg)
{
    state = GAME;

    reg.logic_active = false;
    reg.collisions_active = false;

    std::vector<int8_t> msg(1, 0x5);
    client_.send_to_server(msg, msg.size());

    load_client_textures();
    // initMenu();
    initGame();
}

/**
 * @brief A utility function to check in an entity of a certain id is inside a
 * vector
 *
 * @param vec A vector of entities informations
 * @param id The id of the entity to search
 * @return true if the entity is in the vector
 * @return false if the entity is not in the vector
 */
bool isInside(std::vector<EnemyMovedResponse> vec, size_t id) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (it->enemy_id == id)
            return true;
    }
    return false;
}

/**
 * @brief This function checks if the player is entering inputs and sends info
 * to the server accordingly
 */
void Client::sendPlayerInput() {
    if (player_entity_id == -1)
        return;
    component::controllable& con =
        reg.get_components<component::controllable>()[player_entity_id].value(
        );

    if (!con.left && !con.right && !con.up && !con.down)
        return;

    MoveResponse pos;

    pos.type = 0x24;
    pos.player_id = static_cast<int16_t>(player_entity_id);

    if (con.left) {
        pos.direction = LEFT;
    } else if (con.right) {
        pos.direction = RIGHT;
    } else if (con.up) {
        pos.direction = UP;
    } else if (con.down) {
        pos.direction = DOWN;
    }

    std::vector<int8_t> buff = encodeMoveResponse(pos);

    client_.send_to_server(buff, buff.size());
}

/**
 * @brief This function checks if the player is shooting and sends info to the
 * server accordingly
 *
 */
void Client::sendPlayerAction() {
    if (player_entity_id == -1)
        return;

    component::controllable &con =
        reg.get_components<component::controllable>()[player_entity_id].value();

    if (!con.space)
        return;

    ActionResponse pos;

    pos.type = 0x25;
    pos.player_id = static_cast<int16_t>(player_entity_id);

    if (con.space) {
        pos.input = SPACE;
    }

    std::vector<int8_t> buff = encodeActionResponse(pos);

    client_.send_to_server(buff, buff.size());
}

void Client::receiveServerInfo() {
    bool isempty;

    {
        std::lock_guard<std::mutex> lock(mtx);
        isempty = lastmsg.empty();
    }

    if (isempty)
        return;

    MessageHeader e = decodeMessageHeader(lastmsg);
    std::vector<EnemyMovedResponse> s;

    for (int a = 0; a < e.nbr; a++) {
        s.push_back(decodeEnemyMovedResponse(lastmsg));
    }
    entity_states.push_back({e.timeElapsed, s});

    if (entity_states.size() >= 5)
        entity_states.erase(entity_states.begin());
}

Vector2D Client::entityMovementExtrapol(Vector2D pastPos, Vector2D nextPos,
        int64_t now, int64_t pastTime, int64_t nextTime) {

    static auto progLinear = [](
        auto duration_to_now, auto totalDuration,
        int16_t start, int16_t end) {
        return ( ( (end - start) / totalDuration ) * duration_to_now ) + start;
    };



    auto totalDuration  = nextTime - pastTime;
    auto nowDuration = now - pastTime;

    Vector2D newPos = {
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastPos.x, nextPos.x)),
        static_cast<int16_t>(progLinear(nowDuration, totalDuration, pastPos.y, nextPos.y))
    };

}

/**
 * @brief This function returns the name of an entity type
 *
 * @param value the entity type
 * @return std::string
 */
std::string getKey(int value) {
    for (const auto& pair : type_map) {
        if (pair.second == value)
            return pair.first;
    }
    return "";
}

/**
 * @brief This function updates the game state. It gets all the entities sent by
 * the server and checks for changes. If an entity is new, it creates a new
 * entity in the registry. If an entity is updated, it updates the entity in the
 * registry. If an entity is removed, it removes the entity from the registry.
 *
 * @param delta the time since the last update
 */
void Client::runLevel(double delta) {
    if (entity_states.size() != 2)
        return;
    std::pair<
            int64_t,
            std::vector<EnemyMovedResponse>
            >& old = entity_states.front();
    std::pair<
            int64_t,
            std::vector<EnemyMovedResponse>
            >& back = entity_states.at(1);


}

/**
 * @brief Destroy the Client:: Client object
 *
 */
Client::~Client() {}

/**
 * @brief This function initializes the menu
 *
 */
void Client::initMenu()
{
    menu_info.background = factory.make_background();
    menu_info.title = factory.make_title();
    menu_info.start_text = factory.make_start_text();
    menu_info.menu_background_music = factory.make_menu_background_music();

    reg.add_component<component::controllable>(
        menu_info.start_text, component::controllable()
        );
}

/**
 * @brief This function runs the menu
 *
 * @param delta The amount of time elapsed since the last frame
 */
void Client::runMenu(double delta) {

    if (state == MENU) {
        component::controllable &start_text =
            reg.get_components<component::controllable>()[menu_info.start_text].value();

        if (start_text.space) {
            printf("Transition\n");
            state = TRANSITION;
            menu_info.menu_fade_in_rect = factory.make_fade_in_rect();
        }
    }

    if (state == TRANSITION &&
        std::find(
            reg.dead_entities.begin(), reg.dead_entities.end(), menu_info.menu_fade_in_rect
                ) != reg.dead_entities.end()) {
        state = GAME;
        initGame();
    }
}

/**
 * @brief This function initializes the game
 *
 */
void Client::initGame() {
    factory.make_background();
    // factory.make_game_background_music();
}

void Client::run()
{
    networkThread =  std::thread([this]() { client_.run(); });

    clientStarted = std::chrono::steady_clock::now();

    while (win.isOpen()) {
        double dt = frameClock.restart().asSeconds();

        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    win.close();
        }
        if (state == MENU || state == TRANSITION) {
            runMenu(dt);
        }
        if (state == GAME) {
            sendPlayerInput();
            sendPlayerAction();
            receiveServerInfo();
            runLevel(dt);
        }

        reg.run_systems(dt);
    }

    client_.stop();
    networkThread.join();
}