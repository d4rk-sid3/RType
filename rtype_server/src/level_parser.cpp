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

using namespace component;

using namespace libconfig;

void Server::loadLevel(const std::string &path)
{
    Config conf;
    conf.readFile(path);
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

void Server::logGameEntities()
{
    std::string special_entities = {"background"};

    counter = 0;
    result.clear();

    for (size_t i = 0; i < reg.getEntityNum(); i++) {
        try {
            position &pos = reg.get_components<component::position>()[entity(i)].value();
            name &name_ = reg.get_components<component::name>()[entity(i)].value();

            // Ignore special entities
            // if (std::find(special_entities.begin(), special_entities.end(), name_._name) != special_entities.end()) {
            //     continue;
            // }

            // Clean up out of screen entities
            if (pos.x < -200 || pos.x > 1000) {
                reg.kill_entity(entity(i));
                continue;
            }

            ++counter;

            vector<uint8_t> tmp = encodeEnemyMovedResponse({0x32, static_cast<uint16_t>(i), TYPE_1,
                {static_cast<uint16_t>(pos.x), static_cast<uint16_t>(pos.y)}});

            result.insert(result.end(), tmp.begin(), tmp.end());
        } catch (...) {
        }
    }
    vector<uint8_t> tmp = encodeNbrEntity({0x33, static_cast<uint8_t>(counter)});
    result.insert(result.begin(), tmp.begin(), tmp.end());
}

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


    // for (auto it = entities.begin(); it != entities.end();) {
    //     auto &entity = *it;

    //     // Continue if the entity is not spawned yet
    //     if (it->entity_id == -1) {
    //         ++it;
    //         continue;
    //     }

    //     // Get rid of dead entities
    //     if (std::find(reg.dead_entities.begin(), reg.dead_entities.end(), entity.entity_id) != reg.dead_entities.end()) {
    //         it = entities.erase(it);
    //         continue;-+
    //     }
}
