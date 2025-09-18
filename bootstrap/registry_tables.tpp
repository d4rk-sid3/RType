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
vector<optional<Component>> &registry::register_components()
{
    // get the component type
    type_index new_type = typeid(Component);

    // Check if the corresponding type table is already stored or not
    if (_components_arrays.find(new_type) == _components_arrays.end()) {
        // If not found, create the component table
        _components_arrays[new_type] = vector<optional<Component>>();
    }

    // Get the final table
    auto &table = std::any_cast<std::vector<std::optional<Component>>&>(_components_arrays[new_type]);

    // Create a lambda that will erase this type of component
    // for a given entity
    auto delete_func = [&table] (const entity &e) {
        if (e < table.size())
            table[e].reset();
    };

    // Add the lambda to the erasing functions container
    _erase_functions[new_type] = delete_func;

    // Return the reference to the final table (newly created or not)
    return table;
}

template<typename Component>
vector<optional<Component>> &registry::get_components()
{
    // Get the corresponding table as an any
    any table = _components_arrays[typeid(Component)];

    // Casting the any into its real type
    return any_cast<vector<optional<Component>>>(table);
}

template<typename Component>
vector<optional<Component>> const &registry::get_components() const
{
    // Get the corresponding table as an any
    const any table = _components_arrays.at(typeid(Component));
    const vector<optional<Component>> &table_ref(any_cast<vector<optional<Component>>>(table));

    // Casting the any into its real type
    return table_ref;
}
