/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     commands                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#ifndef INCLUDED_COMMANDS_HPP
    #define INCLUDED_COMMANDS_HPP

    #include <cstdint>
    #include <vector>
    #include <iostream>
    #include <cstring>

    enum Direction : uint8_t {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };

    enum BULLET_TYPE : uint8_t {
        SOLIDE = 0,
        NONE = 1
    };

    struct Vector2D {
        uint16_t x;
        uint16_t y;
    };

    struct Timestamp {
        uint64_t milliseconds;
    };

    // Client -> Serveur 
    struct MoveRequest {
        uint8_t type;               // 0x23
        Direction direction;
    };

    // Serveur -> Client
    struct MoveResponse {
        uint8_t type;               // 0x24
        uint32_t player_id;
        Vector2D direction;
        Vector2D position;
        u_int8_t speed;
        Timestamp timestamp;
    };

    // Client -> Serveur 
    struct ShootRequest {
        uint8_t type;               // 0x25
    };

    // Serveur -> Client
    struct ShootResponse {
        uint8_t type;               // 0x26
        u_int32_t player_id;
        Vector2D bullet_position;
        Vector2D bullet_direction;
        u_int8_t bullet_speed;
        BULLET_TYPE bullet;
    };

    // Client -> Serveur
    struct PickupItemResquest {
        uint8_t type;               // ??
    };


    // Serveur -> Client
    struct PickupItemResponse {
        uint8_t type;               // 0x27
        uint32_t player_id;
        uint32_t item_id;
        Vector2D item_position;
        Timestamp timestamp;
    };


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
