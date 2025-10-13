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
void load_client_textures(void)
{
    ResourceManager::Instance().load("assets/sprites/player/player1.gif", "player1", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/player/player2.gif", "player2", TEXTURE);
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
    ResourceManager::Instance().load("assets/sprites/enemies/boss.gif", "boss", TEXTURE);

    ResourceManager::Instance().load("assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT);
}

/**
 * @brief Construct a new Client:: Client object
 * 
 * @param p The port to connect to
 * @param address The server address
 * @param reg A reference to the registry which will let the game take place
 */
Client::Client(int p, std::string address, registry& reg): port_(p), client_(p, address, std::ref(lastmsg), std::ref(mtx)),  _reg(reg)
{
    load_client_textures();
    //initMenu();
    initGame();
}
/**
 * @brief A utility function to check in an entity of a certain id is inside a vector
 * 
 * @param vec A vector of entities informations
 * @param id The id of the entity to search
 * @return true if the entity is in the vector
 * @return false if the entity is not in the vector
 */
bool isInside(std::vector<EnemyMovedResponse> vec, size_t id)
{
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
std::vector<EnemyMovedResponse> Client::recupAllEntities()
{
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

        for (auto &a : s) {
            std::cout << "Enemy_Type: "  << static_cast<EnemyType>(a.enemy_type) << " ";
            std::cout << "Enemy_Pos_x: "  << static_cast<int16_t>(a.position.x) << " ";
            std::cout << "Enemy_Pos_y: "  << static_cast<int16_t>(a.position.y) << std::endl;
        }
        return s;

    } else {
        std::vector<EnemyMovedResponse> tmp;

        return tmp;
    }
}

/**
 * @brief This function checks if the player is entering inputs and sends info to the server
 * accordingly
 */
void Client::sendPlayerInput()
{
    if (player_entity_id == -1)
        return;
    component::controllable &con = _reg.get_components<component::controllable>()[player_entity_id].value();

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

    for (auto &a : buff) {
        std::cout << static_cast<int>(a) << " " ;

    }
    client_.send_to_server(buff, buff.size());
    
}

/**
 * @brief This function checks if the player is shooting and sends info to the server accordingly
 * 
 */
void Client::sendPlayerAction()
{
    if (player_entity_id == -1)
        return;

    component::controllable &con = _reg.get_components<component::controllable>()[player_entity_id].value();

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
std::string getKey(int value)
{
    for (const auto& pair : type_map) {
        if (pair.second == value)
            return pair.first;
    }
    return "";
}

/**
 * @brief This function updates the game state. It gets all the entities sent by the server and checks for changes.
 * If an entity is new, it creates a new entity in the registry. If an entity is updated, it updates the entity in the registry.
 * If an entity is removed, it removes the entity from the registry.
 * 
 * @param delta the time since the last update
 */
void Client::runLevel(double delta)
{
    static bool first_call = true;
    Factory fac(_reg);
    sendPlayerInput();
    sendPlayerAction();

    new_vec = recupAllEntities();
    printf("Entities received: %d\n", new_vec.size());
    if (new_vec.size() == 0) {
        return;
    }

    for (auto it = new_vec.begin(); it != new_vec.end(); it++) {
        auto &entity = *it;
        try {
        if (first_call) {
            if (getKey(entity.enemy_type) == "player1") {
                old.push_back(entity);
                ids_assoc[entity.enemy_id] = fac.make_entity(getKey(entity.enemy_type));
                player_entity_id = ids_assoc[entity.enemy_id];
                printf("Player received and created\n");
            }
        }
        if (isInside(old, entity.enemy_id)) {
            printf("Entity %d already exists. Updating\n", entity.enemy_id);
            auto &pos = _reg.get_components<component::position>()[ids_assoc[entity.enemy_id]].value();
            printf("Update successful\n");
            pos.x = entity.position.x;
            pos.y = entity.position.y;
        } else {
            printf("Entity %d does not exist. Creating\n", entity.enemy_id);
            printf("Creating a : %s of type: %d\n", (getKey(entity.enemy_type)).c_str(), entity.enemy_type);
            ids_assoc[entity.enemy_id] = fac.make_entity(getKey(entity.enemy_type));
            auto &pos = _reg.get_components<component::position>()[ids_assoc[entity.enemy_id]].value();
            pos.x = entity.position.x;
            pos.y = entity.position.y;
        }
        } catch (std::exception &e) {    
        }
    }
    
    for (auto it = old.begin(); it != old.end(); it++) {
        auto &entity = *it;
        printf("Current entity real id: %d\n", ids_assoc[entity.enemy_id]);
        if (!isInside(new_vec, entity.enemy_id)) {
            if (getKey(entity.enemy_type) == "player1") {
                player_entity_id = -1;
            }
            printf("Entity %d does not exist anymore. Killing\n", entity.enemy_id);
            try {
            _reg.kill_entity((class entity)(ids_assoc[entity.enemy_id]));
            } catch (std::exception &e) {

            }
            printf("Entity %d killed\n", entity.enemy_id);
        }
    }

    old = new_vec;
    first_call = false;
}

/**
 * @brief Destroy the Client:: Client object
 * 
 */
Client::~Client()
{

}

/**
 * @brief This function initializes the menu
 * 
 */
void Client::initMenu()
{
    Factory fac(_reg);
    menu_info.background = fac.make_background();
    menu_info.title = fac.make_title();
    menu_info.start_text = fac.make_start_text();
    menu_info.menu_background_music = fac.make_menu_background_music();

    _reg.add_component<component::controllable>(menu_info.start_text, component::controllable());
}

/**
 * @brief This function runs the menu
 * 
 * @param delta The amount of time elapsed since the last frame
 */
void Client::runMenu(double delta)
{
    Factory fac(_reg);

    if (state == MENU) {
        component::controllable &start_text = _reg.get_components<component::controllable>()[menu_info.start_text].value();

        if (start_text.space) {
            state = TRANSITION;
            menu_info.menu_fade_in_rect = fac.make_fade_in_rect();
        }
    }

    if (state == TRANSITION && std::find(_reg.dead_entities.begin(), _reg.dead_entities.end(), menu_info.menu_fade_in_rect) != _reg.dead_entities.end()) {
        state = GAME;
        initGame();
    }
}

/**
 * @brief This function initializes the game
 * 
 */
void Client::initGame()
{
    Factory factory(_reg);
    factory.make_background();

    //player_entity_id = (int)factory.make_entity("player1");

    // auto &pos = _reg.get_components<component::position>()[player_entity_id].value();
    // pos.x = 50;
    // pos.y = 150;
    factory.make_game_background_music();
    // factory.make_ceiling();
    // factory.make_floor();
    // _reg.kill_entity(menu_info.background);
    // _reg.kill_entity(menu_info.title);
    // _reg.kill_entity(menu_info.start_text);
    // _reg.kill_entity(menu_info.menu_background_music);
}
