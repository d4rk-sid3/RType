/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - r-type_bs                                                          *
 * Description     -                                                                    *
 *     registry                                                                         *
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

#ifndef INCLUDED_REGISTRY_HPP
    #define INCLUDED_REGISTRY_HPP

#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <optional>
#include <functional>
#include <any>
#include <algorithm>
#include "entity.hpp"
#include "exceptions.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

class registry {
    public:
        registry();

        /* Tables management */
        template<typename Component>
        vector<optional<Component>> &register_components();
        
        template<typename Component>
        vector<optional<Component>> &get_components();
        
        template<typename Component>
        vector<optional<Component>> const &get_components() const;

        /* Single component management */
        template<typename Component>
        void remove_component(entity const &from);

        template<typename Component>
        Component &add_component(entity const &to, Component &&c);
    
        /* Entities managament */
        entity spawn_entity();
        void kill_entity(const entity &e);

        /* Systems managment */
        void add_system(const function<void()> &system);
        void run_systems(void);

        /* Getters */
        size_t getEntityNum(void) const {
            return entity_num;
        }

        size_t getComponentNum(void) const {
            return _components_arrays.size();
        }

        size_t getEraseFunctionNum(void) const {
            return _erase_functions.size();
        }

        std::optional<sf::RenderWindow> &get_window() {
            return window;
        }

        std::optional<sf::Event> &get_event() {
            return event;
        }
    
    private:
        unordered_map<type_index, any> _components_arrays;
        unordered_map<type_index, function<void(const entity &)>> _erase_functions;

        vector<function<void()>> _systems;

        NetworkManager _network_manager;

        vector<entity> dead_entities;
        size_t entity_num = 0;

        std::optional<sf::RenderWindow> window;
        std::optional<sf::Event> event;

};

#include "registry_single_comp.tpp"
#include "registry_tables.tpp"

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
