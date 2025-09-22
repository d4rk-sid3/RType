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
    auto &positions = reg.get_components<component::position>();
    auto &velocities = reg.get_components<component::velocity>();

        for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
            try {
                auto &pos = positions.at(i);
                auto &vel = velocities.at(i);

                if (pos && vel) {
                    pos.value().setPosition( vel.value().vx,  vel.value().vy);
                    cout << i << " : Position = {" << pos.value().x << " ," << pos.value().y
                            << "}, Velocity = {" << vel.value().vx << " ," << vel.value().vy
                            << "}" << std::endl;
                }
            } catch (...) {
            }
        }
}

void draw_system(registry &reg, sf::RenderWindow &window)
{
    auto &positions = reg.get_components<component::position>();
    auto &draws = reg.get_components<component::drawable>();

    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            auto &pos = positions.at(i);
            auto &draw = draws.at(i);

            if (pos && draw) {
                auto tmp =  draw.value();

                tmp.sprite.setPosition(pos.value().x, pos.value().y);
                window.draw(tmp.sprite);
            }
        } catch (...) {
        }
    }
}
