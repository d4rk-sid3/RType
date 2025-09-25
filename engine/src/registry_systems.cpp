/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * registry *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄ * ▐
 * ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀ * █▄▄▄▄▄  ▐
 * █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             * █    ▌
 * █          █        █        █    ▌    █         █   █             * ▄▀▄▄▄▄
 * ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             * █    ▐
 * █         █       █ █          █    ▐   █     ▐   █   █               * ▐ ▐
 * ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file registry_systems.cpp
 * @author Farouk OKANLA
 * @brief This file contains the definition of the registry functions that
 * handle the systems
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "registry.hpp"
#include "systems.hpp"

void registry::register_all_systems() {
    register_components<component::position>();
    register_components<component::velocity>();
    register_components<component::controllable>();
    register_components<component::hurtbox>();
    register_components<component::hitbox>();

    add_system(
        [this, &controllables = this->get_components<component::controllable>(),
         &velocities = this->get_components<component::velocity>()]() {
            control_system(*this, controllables, velocities);
        }
    );

    add_system([this, &positions = this->get_components<component::position>(),
                &velocities = this->get_components<component::velocity>()]() {
        position_system(*this, positions, velocities);
    });

    add_system([this, &positions = this->get_components<component::position>(),
                &hurtboxes = this->get_components<component::hurtbox>(),
                &hitboxes = this->get_components<component::hitbox>()]() {
        collision_system(*this, positions, hurtboxes, hitboxes);
    });
}

/**
 * @brief Construct a new registry::registry object. This version doesn't use a
 * window and doesn't register the drawable system
 *
 */
registry::registry() : tmp(sf::RenderWindow()), window(tmp) {
    register_all_systems();
}

/**
 * @brief Construct a new registry::registry object. This version takes a
 * renderwindow as parameter and registers the drawable system
 *
 * @param _window
 */
registry::registry(sf::RenderWindow& _window) : window(_window) {
    register_all_systems();

    register_components<component::drawable>();

    add_system([this, &window = this->window,
                &positions = this->get_components<component::position>(),
                &draws = this->get_components<component::drawable>()]() {
        draw_system(*this, window, positions, draws);
    });
}

/**
 * @brief Add a system to the registry. The best way to add a system is by
 * passing a lambda capturing by reference all the needed components
 *
 * @param system a lambda capturing by reference all the needed components
 */
void registry::add_system(const function<void()>& system) {
    _systems.push_back(system);
}

/**
 * @brief This function runs all the systems registered in the registry
 *
 */
void registry::run_systems() {
    for (const auto& system : _systems) {
        system();
    }
}
