/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     Factory                                                                          *
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

#include "../include/Factory.hpp"

#define PLAYER_MISSISLE_SPEED 10
#define ENEMY_MISSISLE_SPEED 5
#define SCROLL_SPEED 5
#define PLAYER_SPEED 5

Factory::Factory(registry &_reg) : reg(_reg) {}

entity Factory::make_player()
{
    entity player_id = reg.spawn_entity();

    auto &player_sprite = reg.add_component<component::drawable>(player_id, component::drawable());
    player_sprite.setTextureFromMain("assets/sprites/player/player.gif");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(player_id, component::controllable());
}

entity Factory::make_player_missile()
{
    entity missile_id = reg.spawn_entity();

    auto &missile_sprite = reg.add_component<component::drawable>(missile_id, component::drawable());
    missile_sprite.setTextureFromMain("assets/sprites/player/player_missile.gif");

    reg.add_component<component::position>(missile_id, {0, 0});
    reg.add_component<component::velocity>(missile_id, {PLAYER_MISSISLE_SPEED, 0});
}

entity Factory::make_enemy_missile()
{
    entity missile_id = reg.spawn_entity();

    auto &missile_sprite = reg.add_component<component::drawable>(missile_id, component::drawable());
    missile_sprite.setTextureFromMain("assets/sprites/enemies/enemy_missile.gif");

    reg.add_component<component::position>(missile_id, {0, 0});
    reg.add_component<component::velocity>(missile_id, {ENEMY_MISSISLE_SPEED, 0});
}

entity Factory::make_red_trooper()
{
    entity troper_id = reg.spawn_entity();

    auto &troper_sprite = reg.add_component<component::drawable>(troper_id, component::drawable());
    troper_sprite.setTextureFromMain("assets/sprites/enemies/red_trooper.gif");

    reg.add_component<component::position>(troper_id, {0, 0});
    reg.add_component<component::velocity>(troper_id, {0, 0});
}

entity Factory::make_walker()
{
    entity walker_id = reg.spawn_entity();

    auto &troper_sprite = reg.add_component<component::drawable>(walker_id, component::drawable());
    troper_sprite.setTextureFromMain("assets/sprites/enemies/walker.gif");

    reg.add_component<component::position>(walker_id, {0, 0});
    reg.add_component<component::velocity>(walker_id, {0, 0});
}

entity Factory::make_ceiling()
{
    entity ceiling_id = reg.spawn_entity();

    auto &ceiling_sprite = reg.add_component<component::drawable>(ceiling_id, component::drawable());
    ceiling_sprite.setTextureFromMain("assets/sprites/background/ceiling.gif");

    reg.add_component<component::position>(ceiling_id, {0, 0});
    reg.add_component<component::velocity>(ceiling_id, {0, 0});
}

entity Factory::make_floor()
{
    entity floor_id = reg.spawn_entity();

    auto &floor_sprite = reg.add_component<component::drawable>(floor_id, component::drawable());
    floor_sprite.setTextureFromMain("assets/sprites/background/floor.gif");

    reg.add_component<component::position>(floor_id, {0, 0});
    reg.add_component<component::velocity>(floor_id, {0, 0});
}

entity Factory::make_wall()
{
    entity wall_id = reg.spawn_entity();

    auto &wall_sprite = reg.add_component<component::drawable>(wall_id, component::drawable());
    wall_sprite.setTextureFromMain("assets/sprites/bakcground/wall.gif");

    reg.add_component<component::position>(wall_id, {0, 0});
    reg.add_component<component::velocity>(wall_id, {0, 0});
}

entity Factory::make_background()
{
    entity background_id = reg.spawn_entity();

    auto &background_sprite = reg.add_component<component::drawable>(background_id, component::drawable());
    background_sprite.setTextureFromMain("assets/sprites/background/background.jpg");

    reg.add_component<component::position>(background_id, {0, 0});
    reg.add_component<component::velocity>(background_id, {0, 0});
}
