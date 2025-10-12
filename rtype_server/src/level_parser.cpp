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

using namespace libconfig;

void Server::loadLevel(const std::string &path)
{
    Config conf;
    conf.readFile(path);
    printf("Trying to get root\n");
    Setting &root = conf.getRoot();
    
    printf("Got root\n");
    Setting &level_entities = root.lookup("entities");

    printf("Lookup success\n");
    entities.clear();

    for (auto &entity : level_entities) {
        entity_info_t info;
        printf("Entity name: %s\n", (const char *)entity["name"]);
        info.type = entity["name"].c_str();
        info.spawn_time = entity["spawn_time"];
        info.spawn_y = entity["y"];
        entities.push_back(info);
    }
}

void Server::runLevel(double delta)
{
    levelTimer += delta;

    for (auto it = entities.begin(); it != entities.end(); it++) {
        auto &en = *it;
        
        if (en.spawn_time <= levelTimer && en.entity_id.getId() == -1) {
            entity new_id = factory.make_entity(en.type);
            en.entity_id = new_id;
            active_entities.push_back(new_id);
            auto &pos = reg.get_components<component::position>()[new_id].value();
            pos.y = en.spawn_y;
            pos.x = 1000;
        }
    }

    for (auto it = active_entities.begin(); it != active_entities.end();) {
        auto &entity = *it;

        if (std::find(reg.dead_entities.begin(), reg.dead_entities.end(), entity) != reg.dead_entities.end()) {
            it = active_entities.erase(it);
            continue;
        }

        auto &pos = reg.get_components<component::position>()[entity].value();

        if (pos.x < -100) {
            reg.kill_entity(entity);
            it = active_entities.erase(it);
        } else {
            ++it;
        }
    }
}
