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
#include "logic_functions.hpp"

#define PLAYER_MISSILE_SPEED 500.0
#define ENEMY_MISSILE_SPEED 5
#define SCROLL_SPEED 5
#define PLAYER_SPEED 5
#define WALKER_SPEED 300

Factory::Factory(registry &_reg) : reg(_reg) {}

entity Factory::make_entity(const std::string &type)
{
    if (type == "player")
        return make_player();
    else if (type == "red_trooper")
        return make_red_trooper();
    else if (type == "walker")
        return make_walker();
    else if (type == "player_missile")
        return make_player_missile();
    else if (type == "enemy_missile")
        return make_enemy_missile();
}

entity Factory::make_player()
{
    entity player_id = reg.spawn_entity();

    auto &player_sprite = reg.add_component<component::drawable>(player_id, component::drawable());
    player_sprite.setTextureFromName("player");

    reg.add_component<component::position>(player_id, {0, 0});
    reg.add_component<component::velocity>(player_id, {0, 0});
    reg.add_component<component::controllable>(player_id, component::controllable());
    reg.add_component<component::logic>(player_id, component::logic{player_logic});

    auto &player_hurtbox = reg.add_component<component::hurtbox>(player_id, component::hurtbox());
    player_hurtbox.group = 1;
    player_hurtbox.health = 1;
    player_hurtbox.width = 32;
    player_hurtbox.height = 16;

    return player_id;
}

entity Factory::make_player_missile()
{
    entity missile_id = reg.spawn_entity();
    auto &missile_sprite = reg.add_component<component::drawable>(missile_id, component::drawable());
    missile_sprite.setTextureFromName("player_missile");
    
    reg.add_component<component::position>(missile_id, {100.0, 100.0});
    reg.add_component<component::velocity>(missile_id, {PLAYER_MISSILE_SPEED, 0.0});
    auto &missile_hitbox = reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 2;
    missile_hitbox.width = 16;
    missile_hitbox.height = 16;
    missile_hitbox.one_shot = true;

    return missile_id;
}

entity Factory::make_enemy_missile()
{
    entity missile_id = reg.spawn_entity();

    auto &missile_sprite = reg.add_component<component::animated_drawable>(missile_id, component::animated_drawable());
    missile_sprite.setTextureFromName("enemy_missile");
    missile_sprite.setFrameRect(17, 16);
    missile_sprite.frame_duration = 0.25;

    reg.add_component<component::position>(missile_id, {0, 0});
    reg.add_component<component::velocity>(missile_id, {ENEMY_MISSILE_SPEED, 0});
    auto &missile_hitbox = reg.add_component<component::hitbox>(missile_id, component::hitbox());
    missile_hitbox.damage = 10;
    missile_hitbox.targeted_group = 1;
    missile_hitbox.width = 16;
    missile_hitbox.height = 16;
    missile_hitbox.one_shot = true;

    return missile_id;
}

entity Factory::make_red_trooper()
{
    entity troper_id = reg.spawn_entity();

    auto &troper_sprite = reg.add_component<component::animated_drawable>(troper_id, component::animated_drawable());
    troper_sprite.setFrameRect(33, 36);
    troper_sprite.frame_duration = 0.25;
    troper_sprite.setTextureFromName("red_trooper");

    reg.add_component<component::position>(troper_id, {0, 0});
    reg.add_component<component::velocity>(troper_id, {-WALKER_SPEED, 0});
    reg.add_component<component::logic>(troper_id, component::logic{red_trooper_logic});
    auto &troper_hurtbox = reg.add_component<component::hurtbox>(troper_id, component::hurtbox());
    troper_hurtbox.group = 2;
    troper_hurtbox.health = 20;
    troper_hurtbox.width = 33;
    troper_hurtbox.height = 36;

    auto &troper_hitbox = reg.add_component<component::hitbox>(troper_id, component::hitbox());
    troper_hitbox.targeted_group = 1;
    troper_hitbox.damage = 20;
    troper_hitbox.width = 33;
    troper_hitbox.height = 36;
    troper_hitbox.one_shot = false;

    return troper_id;
}

entity Factory::make_walker()
{
    entity walker_id = reg.spawn_entity();

    auto &walker_sprite = reg.add_component<component::animated_drawable>(walker_id, component::animated_drawable());
    walker_sprite.setFrameRect(33, 33);
    walker_sprite.frame_duration = 0.25;
    walker_sprite.setTextureFromName("walker");

    reg.add_component<component::position>(walker_id, {0, 0});
    reg.add_component<component::velocity>(walker_id, {-WALKER_SPEED, 0});

    return walker_id;
}

entity Factory::make_explosion()
{
    entity explosion_id = reg.spawn_entity();

    reg.add_component<component::position>(explosion_id, {0, 0});
    auto &explosion_sprite = reg.add_component<component::animated_drawable>(explosion_id, component::animated_drawable());
    explosion_sprite.setTextureFromName("explosion");
    explosion_sprite.sprite.setScale(0.33, 0.33);
    explosion_sprite.one_shot = true;
    explosion_sprite.setFrameRect(96, 96);
    explosion_sprite.frame_duration = 0.1;

    return explosion_id;
}

entity Factory::make_ceiling()
{
    entity ceiling_id = reg.spawn_entity();

    auto &ceiling_sprite = reg.add_component<component::drawable>(ceiling_id, component::drawable());
    ceiling_sprite.setTextureFromName("assets/sprites/background/ceiling.gif");

    reg.add_component<component::position>(ceiling_id, {0, 0});
    reg.add_component<component::velocity>(ceiling_id, {0, 0});
}

entity Factory::make_floor()
{
    entity floor_id = reg.spawn_entity();

    auto &floor_sprite = reg.add_component<component::drawable>(floor_id, component::drawable());
    floor_sprite.setTextureFromName("assets/sprites/background/floor.gif");

    reg.add_component<component::position>(floor_id, {0, 0});
    reg.add_component<component::velocity>(floor_id, {0, 0});
}

entity Factory::make_wall()
{
    entity wall_id = reg.spawn_entity();

    auto &wall_sprite = reg.add_component<component::drawable>(wall_id, component::drawable());
    wall_sprite.setTextureFromName("assets/sprites/bakcground/wall.gif");

    reg.add_component<component::position>(wall_id, {0, 0});
    reg.add_component<component::velocity>(wall_id, {0, 0});
}

entity Factory::make_background()
{
    entity background_id = reg.spawn_entity();

    auto &background_sprite = reg.add_component<component::drawable>(background_id, component::drawable());
    background_sprite.setTextureFromName("assets/sprites/background/background.jpg");

    reg.add_component<component::position>(background_id, {0, 0});
    reg.add_component<component::velocity>(background_id, {0, 0});
}
