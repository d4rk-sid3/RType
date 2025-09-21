/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     registry                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "registry.hpp"

entity registry::spawn_entity()
{
    // If there's a dead entity id, just re-use it
    if (dead_entities.size() > 0) {
        entity last_id = dead_entities[dead_entities.size() - 1];
        dead_entities.pop_back();
        return last_id;
    } else {
        // Else increment the number of ids
        entity_num += 1;
        return entity(entity_num - 1);
    }
}

void registry::kill_entity(const entity &e)
{
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), e) != dead_entities.end()
        || (size_t)e >= entity_num)
        throw NonExistentEntityID();

    // Add the entity to the dead_entities
    dead_entities.push_back(e);

    // Call each eraser function to delete all related components
    for (const auto& [key, value] : _erase_functions) {
        value(e);
    }
}
