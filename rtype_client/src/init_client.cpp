/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

#include "../include/client.hpp"
#include "components.hpp"

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

Client::Client(int p, std::string address, registry& reg): port_(p), client_(p, address, std::ref(lastmsg), std::ref(mtx)),  _reg(reg)
{
    load_client_textures();
    //initMenu();
    initGame();
}

bool isInside(std::vector<EnemyMovedResponse> vec, size_t id)
{
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (it->enemy_id == id)
            return true;
    }
    return false;
}

std::vector<EnemyMovedResponse> Client::recupAllEntities()
{
    if (!lastmsg.empty()) {    
        std::lock_guard<std::mutex> lock(mtx);  
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

        lastmsg.clear();

        return s;
    } else {
        std::vector<EnemyMovedResponse> tmp;

        return tmp;
    }
}

void Client::sendPlayerInput()
{
    if (player_entity_id == -1)
        return;
    component::controllable &con = _reg.get_components<component::controllable>()[player_entity_id].value();

    if (!con.left && !con.right && !con.up && !con.down && !con.space)
        return;

    MoveResponse pos;

    pos.type = 0x24;
    pos.player_id = static_cast<int16_t>(player_entity_id);

    if (con.left) {
        pos.direction = LEFT;
    } else if (con.right) {
        pos.direction = RIGHT;
    } else if (con.up) {
        printf("UP\n");
        pos.direction = UP;
    } else if (con.down) {
        printf("DOWN\n");
        pos.direction = DOWN;
    }

    std::vector<int8_t> buff = encodeMoveResponse(pos);

    std::cout << "BUFF:" << " ";

    for (auto &a : buff) {
        std::cout << static_cast<int>(a) << " " ;

    }
    client_.send_to_server(buff, buff.size());
    
}

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
        printf("SHOOTING\n");
        pos.input = SPACE;
    }

    std::vector<int8_t> buff = encodeActionResponse(pos);

    std::cout << "BUFF:" << " ";

    client_.send_to_server(buff, buff.size());    
}


std::string getKey(int value)
{
    for (const auto& pair : type_map) {
        if (pair.second == value)
            return pair.first;
    }
    return "";
}

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
    }
    
    for (auto it = old.begin(); it != old.end(); it++) {
        auto &entity = *it;
        printf("Current entity real id: %d\n", ids_assoc[entity.enemy_id]);
        if (!isInside(new_vec, entity.enemy_id)) {
            printf("Entity %d does not exist anymore. Killing\n", entity.enemy_id);
            _reg.kill_entity((class entity)(ids_assoc[entity.enemy_id]));
            printf("Entity %d killed\n", entity.enemy_id);
        }
    }


    old = new_vec;
    first_call = false;
}

Client::~Client()
{

}

void Client::initMenu()
{
    Factory fac(_reg);
    menu_info.background = fac.make_background();
    menu_info.title = fac.make_title();
    menu_info.start_text = fac.make_start_text();
    menu_info.menu_background_music = fac.make_menu_background_music();

    _reg.add_component<component::controllable>(menu_info.start_text, component::controllable());
}

void Client::runMenu(double delta)
{
    Factory fac(_reg);

    if (state == MENU) {
        component::controllable &start_text = _reg.get_components<component::controllable>()[menu_info.start_text].value();

        if (start_text.space) {
            printf("Transition\n");
            state = TRANSITION;
            menu_info.menu_fade_in_rect = fac.make_fade_in_rect();
        }
    }

    if (state == TRANSITION && std::find(_reg.dead_entities.begin(), _reg.dead_entities.end(), menu_info.menu_fade_in_rect) != _reg.dead_entities.end()) {
        state = GAME;
        initGame();
    }
}

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
