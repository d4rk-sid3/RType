/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** init_client
*/

#include "../include/client.hpp"

void load_client_textures(void)
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
    ResourceManager::Instance().load("assets/sprites/background/black.png", "black", TEXTURE);
    ResourceManager::Instance().load("assets/sprites/background/wall1_shadow.png", "ceiling", TEXTURE);

    ResourceManager::Instance().load("assets/fonts/ARCADECLASSIC.TTF", "arcade", FONT);
}

Client::Client(int p, std::string address, registry& reg): port_(p), client_(8080, address, std::ref(lastmsg)),  _reg(reg)
{
    load_client_textures();
    //initMenu();
    initGame();
}

// ... *isInside(std::vector<...> vec, size_t id)
// {
//     for (auto it = vec.begin(); it != vec.end(); it++) {
//         if (it->entity_id.getId() == id)
//             return true;
//     }
//     return false;
// }

void Client::sendPlayerInput()
{

}

std::vector<EnemyMovedResponse> Client::recupAllEntities()
{
    NbrEntity e = decodeNbrEntity(lastmsg);

    std::vector<EnemyMovedResponse> s;

    for (int a = 0; a < e.nbr; a++) {
        s.push_back(decodeEnemyMovedResponse(lastmsg));
    }

    for (auto &a : s) {
        std::cout << "Enemy_Type: "  << a.enemy_type;
        std::cout << "Enemy_Pos_x: "  << a.position.x;
        std::cout << "Enemy_Pos_y: "  << a.position.y << std::endl;
    }

    return s;
}


void Client::runLevel(double delta)
{
    // Factory fac(_reg);
    // getNewEntities();

    // for (auto it = new.begin(); it != new.end();) {
    //     auto &entity = *it;

    //     if (auto old = isInside(old, entity.entity_id.getId())) {
    //         auto &pos = _reg.get_components<component::position>()(ids_assoc[entity.entity_id]).value(); // TODO: get p 
    //         pos.x = entity.position.x;
    //         pos.y = entity.position.y;
    //     } else {
    //         ids_assoc[entity.entity_id] = fac.make_entity(entity.type);
    //         auto &pos = _reg.get_components<component::position>()[ids_assoc[entity.entity_id]].value();
    //         pos.x = entity.position.x;
    //         pos.y = entity.position.y;
    //     }
    // }

    // for (auto it = old.begin(); it != old.end();) {
    //     auto &entity = *it;
    //     if (!isInside(new, entity.entity_id.getId())) {
    //         _reg.kill_entity(entity.entity_id.getId());
    //     }
    // }

    // sendPlayerInput();
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

    player_entity_id = (int)factory.make_entity("player");

    auto &pos = _reg.get_components<component::position>()[player_entity_id].value();
    pos.x = 50;
    pos.y = 150;
    factory.make_game_background_music();
    // factory.make_ceiling();
    // factory.make_floor();
    // _reg.kill_entity(menu_info.background);
    // _reg.kill_entity(menu_info.title);
    // _reg.kill_entity(menu_info.start_text);
    // _reg.kill_entity(menu_info.menu_background_music);
}
