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

/**
 * @brief Construct a new Client:: Client object
 *
 * @param p The port to connect to
 * @param address The server address
 * @param reg A reference to the registry which will let the game take place
 */
Client::Client(int p, std::string address, registry& reg)
    : port_(p), client_(p, address, std::ref(lastmsg), std::ref(mtx)),
      _reg(reg) {
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
 * @brief This functions gets all the entities informations sent by the server
 * and puts them in a vector
 *
 * @return std::vector<EnemyMovedResponse>
 */
std::vector<EnemyMovedResponse> Client::recupAllEntities() {
    bool isempty;

    {
        std::lock_guard<std::mutex> lock(mtx);

        isempty = lastmsg.empty();
    }

    if (!isempty) {
        NbrEntity e = decodeNbrEntity(lastmsg);

        // std::cout << "E: "  << static_cast<int>(e.nbr) << std::endl;

        std::vector<EnemyMovedResponse> s;

        for (int a = 0; a < e.nbr; a++) {
            s.push_back(decodeEnemyMovedResponse(lastmsg));
        }

        for (auto& a : s) {
            std::cout << "Enemy_Type: " << static_cast<EnemyType>(a.enemy_type)
                      << " ";
            std::cout << "Enemy_Pos_x: " << static_cast<int16_t>(a.position.x)
                      << " ";
            std::cout << "Enemy_Pos_y: " << static_cast<int16_t>(a.position.y)
                      << std::endl;
        }
        return s;

    } else {
        std::vector<EnemyMovedResponse> tmp;

        return tmp;
    }
}

/**
 * @brief This function checks if the player is entering inputs and sends info
 * to the server accordingly
 */
void Client::sendPlayerInput() {
    if (player_entity_id == -1)
        return;
    component::controllable& con =
        _reg.get_components<component::controllable>()[player_entity_id].value(
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

    std::cout << "BUFF:" << " ";

    for (auto& a : buff) {
        std::cout << static_cast<int>(a) << " ";
    }
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

    component::controllable& con =
        _reg.get_components<component::controllable>()[player_entity_id].value(
        );

    if (!con.space)
        return;

    ActionResponse pos;

    pos.type = 0x25;
    pos.player_id = static_cast<int16_t>(player_entity_id);

    if (con.space) {
        pos.input = SPACE;
    }

    std::vector<int8_t> buff = encodeActionResponse(pos);

    std::cout << "BUFF:" << " ";

    client_.send_to_server(buff, buff.size());
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
    static bool first_call = true;
    Factory fac(_reg);
    sendPlayerInput();
    sendPlayerAction();

    new_vec = recupAllEntities();
    if (new_vec.size() == 0) {
        return;
    }

    for (auto it = new_vec.begin(); it != new_vec.end(); it++) {
        auto& entity = *it;
        try {
            if (first_call) {
                if (getKey(entity.enemy_type) == "player1") {
                    old.push_back(entity);
                    ids_assoc[entity.enemy_id] =
                        fac.make_entity(getKey(entity.enemy_type));
                    player_entity_id = ids_assoc[entity.enemy_id];
                }
            }
            if (isInside(old, entity.enemy_id)) {
                auto& pos = _reg.get_components<component::position>(
                )[ids_assoc[entity.enemy_id]]
                                .value();
                pos.x = entity.position.x;
                pos.y = entity.position.y;
            } else {
                ids_assoc[entity.enemy_id] =
                    fac.make_entity(getKey(entity.enemy_type));
                auto& pos = _reg.get_components<component::position>(
                )[ids_assoc[entity.enemy_id]]
                                .value();
                pos.x = entity.position.x;
                pos.y = entity.position.y;
            }
        } catch (std::exception& e) {
        }
    }

    for (auto it = old.begin(); it != old.end(); it++) {
        auto& entity = *it;
        if (!isInside(new_vec, entity.enemy_id)) {
            if (getKey(entity.enemy_type) == "player1") {
                player_entity_id = -1;
                player1_dead = true;
            }
            if (getKey(entity.enemy_type) == "player2") {
                player2_dead = true;
            }
            if (getKey(entity.enemy_type) == "boss") {
                boss_dead = true;
            }
            if (getKey(entity.enemy_type) == "small_shooter") {
                boss2_dead = true;
            }
            if (getKey(entity.enemy_type) == "big_shooter") {
                boss1_dead = true;
            }
            if (getKey(entity.enemy_type) == "final_boss") {
                final_boss_dead = true;
            }
            try {
                _reg.kill_entity((class entity)(ids_assoc[entity.enemy_id]));
            } catch (std::exception& e) {
            }
        }
    }

    old = new_vec;
    first_call = false;
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
void Client::initMenu() {
    Factory fac(_reg);
    menu_info.background = fac.make_background();
    menu_info.title = fac.make_title();
    menu_info.start_text = fac.make_start_text();
    menu_info.menu_background_music = fac.make_menu_background_music();

    _reg.add_component<component::controllable>(
        menu_info.start_text, component::controllable()
    );
}

/**
 * @brief This function runs the menu
 *
 * @param delta The amount of time elapsed since the last frame
 */
void Client::runMenu(double delta) {
    Factory fac(_reg);

    if (state == MENU) {
        component::controllable& start_text =
            _reg.get_components<component::controllable>()[menu_info.start_text]
                .value();

        if (start_text.space) {
            state = TRANSITION;
            menu_info.menu_fade_in_rect = fac.make_fade_in_rect();
        }
    }

    if (state == TRANSITION &&
        std::find(
            _reg.dead_entities.begin(), _reg.dead_entities.end(),
            menu_info.menu_fade_in_rect
        ) != _reg.dead_entities.end()) {
        state = LEVEL1;
        initGame();
    }
}

/**
 * @brief This function initializes the game
 *
 */
void Client::initGame() {
    Factory factory(_reg);
    factory.make_background();
    factory.make_game_background_music();
}

void Client::handleSubStates(double delta, sf::RenderWindow& win)
{
    if (state == LEVEL1) {
        if (boss_dead) {
            substate = VICTORY;
        }
    }
    if (state == LEVEL2) {
        if (boss1_dead && boss2_dead) {
            substate = VICTORY;
        }
    }
    if (state == LEVEL3) {
        if (final_boss_dead) {
            substate = VICTORY;
        }
    }
    
    if (substate == VICTORY) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL CLEARED", true);
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_ended) {
            player1_dead = false;
            player2_dead = false;
            if (state == LEVEL1) {
                state = LEVEL2;
                substate = LEVEL2_START;
            } else if (state == LEVEL2) {
                state = LEVEL3;
                substate = LEVEL3_START;
            } else if (state == LEVEL3) {
                substate = SUB_NONE;
                exit(0);
            }
        }
    }

    if (player1_dead && player2_dead && substate != GAME_OVER) {
        substate = GAME_OVER;
        ui_handler = UIHandler("GAME OVER", true);
    }
    if (substate == GAME_OVER) {
        if (ui_handler.fade_started == false) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_in(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
            exit(0);
        }
    }
    if (substate == LEVEL1_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 1", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
    if (substate == LEVEL2_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 2", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
    if (substate == LEVEL3_START) {
        if (ui_handler.fade_started == false) {
            ui_handler = UIHandler("LEVEL 3", false);
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_started) {
            ui_handler.fade_out(win, delta);
        }
        if (ui_handler.fade_ended) {
            substate = SUB_NONE;
        }
    }
}
