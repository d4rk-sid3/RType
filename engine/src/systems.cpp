/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Sep, 2025 * Title           - r-type_bs * Description
 * -                                                                    *
 *     systems *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file systems.cpp
 * @author Farouk OKANLA
 * @brief
 * @version 0.1
 * @date 2025-09-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "components.hpp"
#include "registry.hpp"

void position_system(
    registry& reg, std::vector<optional<component::position>>& positions,
    std::vector<optional<component::velocity>>& velocities
) {

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& pos = positions.at(i);
            auto& vel = velocities.at(i);

            if (pos && vel) {
                pos.value().setPosition(vel.value().vx, vel.value().vy);
            }
        } catch (...) {
        }
    }
}

void draw_system(
    registry& reg, sf::RenderWindow& window,
    std::vector<optional<component::position>>& positions,
    std::vector<optional<component::drawable>>& draws
) {
    window.clear(sf::Color::Black);

    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& pos = positions.at(i);
            auto& draw = draws.at(i);

            if (pos && draw) {
                draw.value().sprite.setPosition(pos.value().x, pos.value().y);
                window.draw(draw.value().sprite);
            }
        } catch (...) {
        }
    }
    window.display();
}

void control_system(
    registry& reg, std::vector<optional<component::controllable>>& controls,
    std::vector<optional<component::velocity>>& velocities
) {
    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            auto& control = controls.at(i);
            auto& vel = velocities.at(i);

            if (control && vel) {
                auto& ctl = control.value();
                auto& vl = vel.value();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && ctl.up)
                    vl.vy = -1;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
                         ctl.down)
                    vl.vy = 1;
                else
                    vl.vy = 0;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && ctl.left)
                    vl.vx = -1;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                         ctl.right)
                    vl.vx = 1;
                else
                    vl.vx = 0;
            }
        } catch (...) {
        }
    }
}

void collision_system(
    registry& reg, std::vector<optional<component::position>>& positions,
    std::vector<optional<component::hurtbox>>& hurtboxes,
    std::vector<optional<component::hitbox>>& hitboxes
) {
    for (size_t i = 0; i < reg.getEntityNum(); ++i) {
        try {
            // Go through each hurtbox and their positions
            auto& pos = positions.at(i);
            auto& hurtbox = hurtboxes.at(i);

            if (pos && hurtbox) {
                for (size_t j = 0; j < reg.getEntityNum(); ++j) {
                    // Go through each hitbox and their positions
                    auto& hit_pos = positions.at(j);
                    auto& hitbox = hitboxes.at(j);

                    if (hit_pos && hitbox) {
                        // Create the hitbox and hurtbox rects with their
                        // dimensions
                        sf::IntRect hurt_rect = sf::IntRect(
                            sf::Vector2i(pos.value().x, pos.value().y),
                            sf::Vector2i(
                                hurtbox.value().width, hurtbox.value().height
                            )
                        );
                        sf::IntRect hit_rect = sf::IntRect(
                            sf::Vector2i(hit_pos.value().x, hit_pos.value().y),
                            sf::Vector2i(
                                hitbox.value().width, hitbox.value().height
                            )
                        );

                        if (hurt_rect.intersects(hit_rect) &&
                            hurtbox.value().group ==
                                hitbox.value().targeted_group) {
                            std::cout << "COLLISION " << i << " " << j << "\n";
                            hurtbox.value().health -= hitbox.value().damage;
                        }
                    }
                }
            }
        } catch (...) {
        }
    }
}