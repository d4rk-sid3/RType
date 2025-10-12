/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     level_parser                                                                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include <libconfig.h++>
#include "server.hpp"
#include "Factory.hpp"
#include "logic_functions.hpp"

/**
 * @brief This file contains the definition of the level handling functions
 * 
 */

using namespace component;

using namespace libconfig;

/**
 * @brief This function loads a level from a configuration file
 * It stores all the entities of the level in a vector with their spawn time
 * and spawn y position
 * 
 * @param path 
 */
void Server::loadLevel(const std::string &path)
{
    Config conf;
    conf.readFile(path.c_str());
    Setting &root = conf.getRoot();
    
    Setting &level_entities = root.lookup("entities");

    entities.clear();

    for (auto &entity : level_entities) {
        entity_info_t info;
        info.type = (std::string)entity["name"];
        info.spawn_time = entity["spawn_time"];
        info.spawn_y = entity["y"];
        entities.push_back(info);
    }
}

/**
 * @brief This functions send to the clients information about each game entity alive
 * 
 */
void Server::logGameEntities()
{
    std::vector<std::string> special_entities = {"background", "menu_background_music"};

    counter = 0;
    result.clear();

    for (size_t i = 0; i < reg.getEntityNum(); i++) {
        try {
            position &pos = reg.get_components<component::position>()[entity(i)].value();
            name &name_ = reg.get_components<component::name>()[entity(i)].value();
            EnemyType type = type_map[name_._name];
            unique_id &uid = reg.get_components<component::unique_id>()[entity(i)].value();

            // Ignore special entities
            if (std::find(special_entities.begin(), special_entities.end(), name_._name) != special_entities.end()) {
                continue;
            }

            // Clean up out of screen entities
            if (pos.x < -200 || pos.x > 1000) {
                if (name_._name != "ceiling" && name_._name != "floor") {
                    reg.kill_entity(entity(i));
                    continue;
                }
            }

            ++counter;

            vector<int8_t> tmp = encodeEnemyMovedResponse({0x37, static_cast<int16_t>(uid), type,
                {static_cast<int16_t>(pos.x), static_cast<int16_t>(pos.y)}});

            std::cout << "Enemy_Type: "  << (type) << " ";
            std::cout << "Enemy_Name: "  << (name_._name) << " ";
            std::cout << "Enemy_Pos_x: "  << static_cast<int>(pos.x) << " ";
            std::cout << "Enemy_Pos_y: "  << static_cast<int>(pos.y) << std::endl;

            result.insert(result.end(), tmp.begin(), tmp.end());
        } catch (...) {

        }
    }
    vector<int8_t> tmp = encodeNbrEntity({0x38, static_cast<int16_t>(counter)});
    result.insert(result.begin(), tmp.begin(), tmp.end());
}

/**
 * @brief This function handles the received players' inputs
 * 
 * @param delta 
 */
void Server::receivePlayerInput(double delta)
{
    static double shoot_timer = 0;

    shoot_timer += delta;

    size_t len = 0;

    {
        std::lock_guard<std::mutex> lock(mtx);
        len = messages.size();
    }

    while (len != 0) {

        std::pair<asio::ip::udp::endpoint, std::vector<int8_t>> msg;

        {
            std::lock_guard<std::mutex> lock(mtx);
            msg = messages.front();
            messages.erase(messages.begin());
            len = messages.size();
        }

        if (std::find_if(all_clients.begin(), all_clients.end(),
                            [msg] (auto& tmp)
                            {
                                return tmp.first == msg.first;
                            }
                        ) == all_clients.end())
        {
            if (all_clients.empty())
                all_clients[msg.first] = player1_entity_id;
            else
                all_clients[msg.first] = player2_entity_id;
            return;
        }

        std::vector<int8_t> tmp = msg.second;

        try {
        if (tmp[0] == 0x24) {
            std::cerr << "INPUT" << std::endl;
            
            velocity &vel = reg.get_components<component::velocity>()[all_clients[msg.first]].value();
            MoveResponse move_info = decodeMoveResponse(tmp);
            
            if (move_info.direction == LEFT) {
                vel.vx = -PLAYER_SPEED;
            }
            if (move_info.direction == RIGHT) {
                vel.vx = PLAYER_SPEED;
            }
            if (move_info.direction == UP) {
                vel.vy = -PLAYER_SPEED;
            }
            if (move_info.direction == DOWN) {
                vel.vy = PLAYER_SPEED;
            }
        
        }
        if (tmp[0] == 0x25) {
            
            std::cerr << "ACTION" << std::endl;
            ActionResponse action_info = decodeActionResponse(tmp);

            if (action_info.input == SPACE && shoot_timer > PLAYER_SHOOT_COOLDOWN) {
                Factory fac(reg);
                shoot_timer = 0;
                entity missile = fac.make_player_missile();
                position &pos = reg.get_components<component::position>()[all_clients[msg.first]].value();
                position &missile_pos = reg.get_components<component::position>()[missile].value();
                missile_pos.x = pos.x + 8;
                missile_pos.y = pos.y + 6;
            }
        }
        } catch (...) {}

    }

}

/**
 * @brief This function runs the current level
 * It makes sure to spawn new entities if their spawn time has come
 * And calls the receivePlayerInput function as well as the logGameEntities function
 * @param delta The amount of time elapsed since the last frame
 */
void Server::runLevel(double delta)
{
    Factory factory(reg);
    levelTimer += delta;

    // Spawn new ready entities
    for (auto it = entities.begin(); it != entities.end(); it++) {
        auto &en = *it;
        
        if (en.spawn_time <= levelTimer && en.entity_id.getId() == -1) {
            en.entity_id = factory.make_entity(en.type);
            auto &pos = reg.get_components<component::position>()[en.entity_id].value();
            pos.y = en.spawn_y;
            pos.x = 1000;
        }
    }

    logGameEntities();
    receivePlayerInput(delta);

    if (!result.empty()) {
        // std::cout << "[SERVER] sending " << result.size() << " bytes" << std::endl;
        // server_.send_to_client(result, result.size(), server_.getLastSender());

        for (auto & tmp : all_clients) {
            server_.send_to_client(result, result.size(), tmp.first);
        }
    }
}
