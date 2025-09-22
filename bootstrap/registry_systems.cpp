/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     registry                                                                         *
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

#include "registry.hpp"
#include "systems.hpp"

registry::registry() : window(std::make_optional<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML Drawable")), event(std::make_optional<sf::Event>())
{
    register_components<component::position>();
    register_components<component::velocity>();
    register_components<component::drawable>();
    register_components<component::controllable>();


    add_system(
        [
            this,
            &event = this->event.value(),
            &controllables = this->get_components<component::controllable>(),
            &velocities = this->get_components<component::velocity>()
        ]
        () {
            control_system(*this, event, controllables, velocities);
        }
    );

    add_system(
        [
            this,
            &positions = this->get_components<component::position>(),
            &velocities = this->get_components<component::velocity>()
        ]
        () {
            position_system(*this, positions, velocities);
        }
    );

    add_system(
        [
            this,
            &window = this->window.value(),
            &positions = this->get_components<component::position>(),
            &draws = this->get_components<component::drawable>()
        ]
        () {
            draw_system(*this, window, positions, draws);
        }
    );
}

void registry::add_system(const function<void()> &system)
{
    _systems.push_back(system);
}

void registry::run_systems(void)
{
    for (const auto& system : _systems) {
        system();
    }
}
