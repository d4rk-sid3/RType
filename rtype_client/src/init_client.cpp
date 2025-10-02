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

Client::Client(int p, std::string address, registry& reg): port_(p), client_(8080, "client"), _reg(reg)
{
    load_client_textures();
    initMenu();

    asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 8080);

    std::thread input_thread([this, server_endpoint]() {
        std::string input;
        while (std::getline(std::cin, input)) {
            MoveRequest resq;
            resq.type = 0x23;

            if (input == "up")
                resq.direction = UP;
            else if (input == "down")
                resq.direction = DOWN;
            else if (input == "left")
                resq.direction = LEFT;
            else if (input == "right")
                resq.direction = RIGHT; 
            else
                continue;
            
            std::vector<uint8_t> buff = encodeMoveResquest(resq);

            this->client_.send(buff, buff.size(), server_endpoint);
        }
    });

    while (1) {

        client_.poll();

        auto msg = client_.getLastMsg();
        if (!msg.first.empty()) {
            MoveResponse res = decodeMoveResponse(msg.first);
            if (res.type == 0x24) {
                std::cout << "Serveur: Player " << res.player_id
                        << " se déplace vers " << res.direction.x << ", " << res.direction.y
                        << std::endl;
            }
        }
    }
    input_thread.detach();
}

void Client::runLevel(double delta)
{
    for (auto it = active_entities.begin(); it != active_entities.end();) {
        auto &entity = *it;

        if (std::find(_reg.dead_entities.begin(), _reg.dead_entities.end(), entity) != _reg.dead_entities.end()) {
            it = active_entities.erase(it);
            continue;
        }

        auto &pos = _reg.get_components<component::position>()[entity].value();
        if (pos.x < -100 || pos.x > 1200) {
            _reg.kill_entity(entity);
            it = active_entities.erase(it);
        } else {
            ++it;
        }
    }
}

MoveRequest Client::getMoveKey()
{
    return move;
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

    player_entity_id = (int)factory.make_entity("player");
    active_entities.push_back((entity)player_entity_id);
    
    printf("Player init\n");
    auto &pos = _reg.get_components<component::position>()[player_entity_id].value();
    pos.x = 50;
    pos.y = 150;
    factory.make_game_background_music();
    factory.make_ceiling();
    factory.make_floor();
    // _reg.kill_entity(menu_info.background);
    _reg.kill_entity(menu_info.title);
    _reg.kill_entity(menu_info.start_text);
    _reg.kill_entity(menu_info.menu_background_music);
}
