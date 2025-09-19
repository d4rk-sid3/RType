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

void position_system_improved(registry &reg,
                                std::vector<optional<component::position>> &positions,
                                std::vector<optional<component::velocity>> &velocities)
{
    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        if (i >= positions.size() || i >= velocities.size())
            continue;

        auto const &pos = positions[i];
        auto const &vel = velocities[i];

        if (pos != nullopt && vel != nullopt) {
            std::cerr << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                << "}, Velocity = {" << vel.value().vx << " ," << vel.value().vy
                << "}" << std::endl;
        }
    }
}

template <class ... Components, typename Function>
void add_system (Function const &f)
{
    auto lambda = [] ()
}