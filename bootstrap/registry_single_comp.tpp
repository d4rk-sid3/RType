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

template<typename Component>
void registry::remove_component(entity const &from)
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), from) != dead_entities.end()
        || (size_t)from >= entity_num)
        throw NonExistentEntityID();

    // Call the eraser function corresponding to the type
    _erase_functions[typeid(Component)](from);
}

template<typename Component>
Component &registry::add_component(entity const &to, Component &&c)
{
    // Check if the type is registered
    if (_components_arrays.find(typeid(Component)) == _components_arrays.end()) {
        throw NonExistentComponentType();
    }
    // Check if the entity is really registered
    if (find(dead_entities.begin(), dead_entities.end(), to) != dead_entities.end()
        || (size_t)to >= entity_num)
        throw NonExistentEntityID();

    // Get the table corresponding to the component type
    vector<optional<Component>> &table = get_components<Component>();

    // Check if the table is large enough
    if (table.size() < (size_t)to + 1) {
        // If not, add enough space to reach the entity's id
        table.resize((size_t)to + 1);
    }
    // Move the component into the entity's place
    table[to].reset();
    table[to] = std::move(c);

    return *table[to];
}
