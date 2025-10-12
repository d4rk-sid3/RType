/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * logics_functions *
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

#ifndef INCLUDED_LOGICS_FUNCTIONS_HPP
#define INCLUDED_LOGICS_FUNCTIONS_HPP

#include "components.hpp"
#include "registry.hpp"

#define PLAYER_MISSISLE_SPEED 10
#define ENEMY_MISSISLE_SPEED 5
#define SCROLL_SPEED 5
#define PLAYER_SPEED 350
#define TROOPER_SPEED_X 150
#define TROOPER_SPEED_Y 60
#define PLAYER_SHOOT_COOLDOWN 0.25
#define ENEMY_SHOOT_COOLDOWN 2
#define BOSS_SHOOT_COOLDOWN 2
#define BOSS_MISSILE_SPEED 7
#define BOSS_SPEED 100

extern bool boss_dead;

void player_logic(double delta, registry& reg, entity entity);
void red_trooper_logic(double delta, registry& reg, entity entity);
void walker_logic(double delta, registry& reg, entity entity);
void boss_logic(double delta, registry& reg, entity entity);
void start_text_logic(double delta, registry& reg, entity entity);
void fade_in_rect_logic(double delta, registry& reg, entity entity);
void fade_out_rect_logic(double delta, registry& reg, entity entity);

#endif

/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * MIT License * Copyright (c) 2025 Anonymous *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * * of this software and associated documentation files (the "Software"), to
 * deal        * in the Software without restriction, including without
 * limitation the rights         * to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell            * copies of the Software, and
 * to permit persons to whom the Software is                * furnished to do
 * so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in
 * all       * copies or substantial portions of the Software. *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE        * SOFTWARE. *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */
