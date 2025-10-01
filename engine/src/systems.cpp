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

#include "../include/registry.hpp"
#include "../include/components.hpp"

void position_system(double delta, registry &reg, std::vector<optional<component::position>> &positions,
                                std::vector<optional<component::velocity>> &velocities)
{
        for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
            try {
                auto &pos = positions.at(i);
                auto &vel = velocities.at(i);

                if (pos && vel) {
                    pos.value().setPosition(vel.value().vx * delta,  vel.value().vy * delta);
                }
            } catch (...) {
            }
        }
}

void draw_system(double delta, registry &reg, sf::RenderWindow &window,
                        std::vector<optional<component::position>> &positions,
                        std::vector<optional<component::drawable>> &draws,
                        std::vector<optional<component::animated_drawable>> &anim_draws,
                        std::vector<optional<component::text>> &texts)
{
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
        try {
            auto &pos = positions.at(i);
            auto &anim_draw = anim_draws.at(i);

            if (pos && anim_draw) {
                anim_draw.value().animate(delta);
                if (anim_draw.value().one_shot && anim_draw.value().done_once) {
                    reg.kill_entity(entity(i));
                    continue;
                } 
                anim_draw.value().sprite.setPosition(pos.value().x, pos.value().y);
                window.draw(anim_draw.value().sprite);
            }
        } catch (...) {
        }
        try {
            auto& pos = positions.at(i);
            auto &text = texts.at(i);

            if (text && pos) {
                text.value().text.setPosition(pos.value().x, pos.value().y);
                window.draw(text.value().text);
            }
        } catch (...) {
        }
    }
    window.display();
}

void control_system(double delta, registry &reg, std::vector<optional<component::controllable>> &controls)
{
    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            if (i >= controls.size() || !controls.at(i).has_value())
                continue;
            auto &control = controls.at(i).value();

            control.getKeyboardInput();
        } catch (...) {
        }
    }
}

void collision_system(double delta, registry &reg, std::vector<optional<component::position>> &positions,
    std::vector<optional<component::hurtbox>> &hurtboxes,
    std::vector<optional<component::hitbox>> &hitboxes)
{
    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            // Go through each hurtbox and their positions
            auto& pos = positions.at(i);
            auto& hurtbox = hurtboxes.at(i);

            if (pos && hurtbox) {
                hurtbox.value() .hurt = false;
                for (size_t j = 0; j < reg.getEntityNum(); ++j) {
                    // Go through each hitbox and their positions
                    auto& hit_pos = positions.at(j);
                    auto& hitbox = hitboxes.at(j);

                    if (hit_pos && hitbox) {
                        // Create the hitbox and hurtbox rects with their dimensions
                        sf::IntRect hurt_rect = sf::IntRect(sf::Vector2i((int)(pos.value().x), (int)(pos.value().y)), sf::Vector2i(hurtbox.value().width, hurtbox.value().height));
                        sf::IntRect hit_rect = sf::IntRect(sf::Vector2i((int)(hit_pos.value().x), (int)(hit_pos.value().y)), sf::Vector2i(hitbox.value().width, hitbox.value().height));

                        if (hurt_rect.intersects(hit_rect) &&
                            hurtbox.value().group ==
                                hitbox.value().targeted_group) {
                            hurtbox.value().hurt = true;
                            std::cout << "COLLISION " << i << " " << j << "\n";
                            hurtbox.value().health -= hitbox.value().damage;

                            if (hitbox.value().one_shot) {
                                reg.kill_entity(entity(j));
                            }
                        }
                    }
                }
            }
        } catch (...) {
        }
    }  
}

void logic_system(double delta, registry &reg, std::vector<optional<component::logic>> &logics)
{
    for (size_t i = 0; i < reg.getEntityNum(); ++ i) {
        try {
            auto &logic = logics.at(i);

            if (logic) {
                logic.value().logic_function(delta, reg, entity(i));
            }
        } catch (...) {
        }
    }
}
