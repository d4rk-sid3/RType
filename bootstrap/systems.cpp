/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     systems                                                                          *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "registry.hpp"
#include "components.hpp"

void position_system(registry &reg)
{
    auto const &positions = reg.get_components<component::position>();
    auto const &velocities = reg.get_components<component::velocity>();


    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        auto const &pos = i < positions.size() ? positions[i] : nullopt;
        auto const &vel = i < velocities.size() ? velocities[i] : nullopt;

        if (pos && vel) {
            std::cerr << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                << "}, Velocity = {" << vel.value().vx << " ," << vel.value().vy
                << "}" << std::endl;
        }
    }
}
