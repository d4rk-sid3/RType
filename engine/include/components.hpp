/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     components                                                                       *
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

#ifndef INCLUDED_COMPONENTS_HPP
    #define INCLUDED_COMPONENTS_HPP

/**
 * @file components.hpp
 * @author Farouk OKANLA
 * @brief This file defines the different types of components available to the engine
 * @version 0.1
 * @date 2025-09-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.hpp"
#include "entity.hpp"

namespace component {
    /**
     * @brief The position component, with an x and y position
     */
    typedef struct position_s {
        double x;
        double y;
        double z;
        void setPosition( double _x, double _y) {
            this->x += _x;
            this->y += _y;
        }
    }position;

    /**
     * @brief The velocity component, with an x and y velocity. This component allows an entity to move
     * 
     */
    typedef struct velocity_s {
        double vx;
        double vy;
    }velocity;

    /**
     * @brief The drawable component, defined by an sf::Sprite. This component allows an entity to be drawn on the screen
     * 
     */
    typedef struct drawable_s {
        sf::Sprite sprite;

        void setTextureFromName(std::string texture_name) {
            sf::Texture &texture = TextureManager::Instance().getTexture(texture_name);
            this->sprite.setTexture(texture);
        }
    }drawable;

    /**
     * @brief The animated_drawable component. It is a drawable component with an animated sprite. It has a frame_rect, a frame_duration and a frame_timer to allow animation
     * 
     */
    typedef struct animated_drawable_s {
        sf::Sprite sprite;
        sf::IntRect frame_rect;
        double frame_duration;
        double frame_timer;

        void setTextureFromName(std::string texture_name) {
            sf::Texture &texture = TextureManager::Instance().getTexture(texture_name);
            this->sprite.setTexture(texture);
        }


        void setFrameRect(int width, int height) {
            this->frame_rect = sf::IntRect(0, 0, width, height);
            this->sprite.setTextureRect(this->frame_rect);
        }

        void animate(double delta) {
            this->frame_timer += delta;

            if (this->frame_timer >= this->frame_duration) {
                this->frame_timer = 0;
                this->frame_rect.left += this->frame_rect.width;
                if (this->frame_rect.left >= this->sprite.getTexture()->getSize().x) {
                    this->frame_rect.left = 0;
                }
                this->sprite.setTextureRect(this->frame_rect);
            }
        }
    }animated_drawable;

    /**
     * @brief The controllable component, defined by a set of booleans. This component allows an entity to be controlled by the user's keyboard input
     * 
     */
    typedef struct controllable_s {
        bool up;
        bool down;
        bool left;
        bool right;
        bool space;
        controllable_s() : up(false), down(false), left(false), right(false), space(false) {}

        void getKeyboardInput() {
            this->up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            this->down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            this->left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            this->right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            this->space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        }
    }controllable;


    /**
     * @brief The hurtbox component, defined by a size, health and a group (1 for the player, 2 for the enemies for example). A hitbox will detect collision with a hurtbox if its targeted group corresponds to the hurtbox group 
     * 
     */
    typedef struct hurtbox_s{
        /**
         * @brief The health of the hurtbox. When it reaches 0, the entity is considered dead
         */
        int health;

        /**
         * @brief The group of the hurtbox. For example, if the hurtbox group is 1 for the player and 2 for the enemies, the player's hitbox should have a targeted_group of 2 to damage enemies
         */
        int group;

        /**
         * @brief The width of the hurtbox
         */
        int width;

        /**
         * @brief The height of the hurtbox
         */
        int height;
    }hurtbox;

    /**
     * @brief The hitbox component, defined by a size, damage and a targeted_group (2 for the player, 1 for the enemies for example). A hitbox will detect collision with a hurtbox if its targeted group corresponds to the hurtbox group 
     * 
     */
    typedef struct hitbox_s{
        /**
         * @brief The damage that will be inflicted to the hurtbox on collision
         */
        int damage;

        /**
         * @brief The group that this hitbox will target. For example, if the hurtbox group is 1 for the player and 2 for the enemies, the player's hitbox should have a targeted_group of 2 to damage enemies
         */
        int targeted_group;

        /**
         * @brief The width of the hitbox
         */
        int width;

        /**
         * @brief The height of the hitbox
         */
        int height;
    }hitbox;

    /**
     * @brief The logic component, defined by a function pointer. This component allows an entity to have a logic defined by the user
     */
    typedef struct logic_s{
        /**
         * @brief A pointer to a function that takes a delta time, a reference to the registry and an entity as parameters and returns void
         * @param delta The time elapsed since the last frame
         * @param reg A reference to the registry
         * @param entity The entity that owns this logic component
         */
        void (*logic_function)(double, class registry &, entity);
    }logic;
    
}

#endif

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2025 Anonymous                                                         *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */
