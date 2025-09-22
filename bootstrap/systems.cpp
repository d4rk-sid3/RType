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

void position_system(registry &reg, std::vector<optional<component::position>> &positions,
                                std::vector<optional<component::velocity>> &velocities)
{

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

void draw_system(registry &reg, sf::RenderWindow &window,
                        std::vector<optional<component::position>> &positions,
                        std::vector<optional<component::drawable>> &draws)
{
    window.clear(sf::Color::Black);

    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            auto &pos = positions.at(i);
            auto &draw = draws.at(i);

            if (pos && draw) {  
                draw.value().sprite.setPosition(pos.value().x, pos.value().y);
                window.draw(draw.value().sprite);
                cout << i << " : Drawing at {" << pos.value().x << " ," << pos.value().y << "}" << std::endl;
            }
        } catch (...) {
        }
    }
    window.display();
}

void control_system(registry &reg, sf::Event &event,
                        std::vector<optional<component::controllable>> &controls,
                        std::vector<optional<component::velocity>> &velocities)
{
    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            auto &control = controls.at(i);
            auto &vel = velocities.at(i);

            if (control && vel) {
                auto &ctl =  control.value();
                auto &vl =  vel.value();

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up && ctl.up)
                    vl.vy = -1;
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down && ctl.down)
                    vl.vy = 1;
                else
                    vl.vy = 0;
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && ctl.left)
                    vl.vx = -1;
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && ctl.right)
                    vl.vx = 1;
                else
                    vl.vx = 0;
            }
        } catch (...) {
        }
    }
}
