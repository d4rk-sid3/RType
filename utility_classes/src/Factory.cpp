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
#include "../../rtype_server/include/server.hpp"

#define PLAYER_MISSILE_SPEED 500.0
#define ENEMY_MISSILE_SPEED 5
#define BACKGROUND_SPEED 50
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

    auto &player_shoot_music = reg.add_component<component::audio>(missile_id, component::audio());
    player_shoot_music.audio.reset(new sf::Music);
    player_shoot_music.audio->openFromFile("assets/audio/player_shoot.wav");
    player_shoot_music.audio->setLoop(false);
    player_shoot_music.audio->play();

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

    auto &enemy_shoot_music = reg.add_component<component::audio>(missile_id, component::audio());
    enemy_shoot_music.audio.reset(new sf::Music);
    enemy_shoot_music.audio->openFromFile("assets/audio/enemy_shoot.wav");
    enemy_shoot_music.audio->setLoop(false);
    enemy_shoot_music.audio->play();

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

    auto &explosion_music = reg.add_component<component::audio>(explosion_id, component::audio());
    explosion_music.audio.reset(new sf::Music);
    explosion_music.audio->openFromFile("assets/audio/explosion.wav");
    explosion_music.audio->setLoop(false);
    explosion_music.audio->play();

    return explosion_id;
}

entity Factory::make_hit_effect()
{
    entity hit_effect_id = reg.spawn_entity();

    reg.add_component<component::position>(hit_effect_id, {0, 0});
    auto &hit_effect_sprite = reg.add_component<component::animated_drawable>(hit_effect_id, component::animated_drawable());
    hit_effect_sprite.setTextureFromName("hit_effect");
    hit_effect_sprite.one_shot = true;
    hit_effect_sprite.setFrameRect(33, 32);
    hit_effect_sprite.frame_duration = 0.05;

    auto &hit_effect_music = reg.add_component<component::audio>(hit_effect_id, component::audio());
    hit_effect_music.audio.reset(new sf::Music);
    hit_effect_music.audio->openFromFile("assets/audio/enemy_shoot.wav");
    hit_effect_music.audio->setLoop(false);
    hit_effect_music.audio->play();

    return hit_effect_id;
}


entity Factory::make_ceiling()
{
    entity ceiling_id = reg.spawn_entity();

    auto &ceiling_sprite = reg.add_component<component::drawable>(ceiling_id, component::drawable());
    ceiling_sprite.setTextureFromName("ceiling");
    ceiling_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 50));
    ResourceManager::Instance().getTexture("ceiling").setRepeated(true);

    reg.add_component<component::position>(ceiling_id, {0, 0});
    reg.add_component<component::velocity>(ceiling_id, {-BACKGROUND_SPEED, 0});

    return ceiling_id;
}

entity Factory::make_floor()
{
    entity floor_id = reg.spawn_entity();

    auto &floor_sprite = reg.add_component<component::drawable>(floor_id, component::drawable());
    floor_sprite.setTextureFromName("ceiling");
    floor_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 50));
    floor_sprite.sprite.setScale(1, -1);
    ResourceManager::Instance().getTexture("ceiling").setRepeated(true);

    reg.add_component<component::position>(floor_id, {0, WINDOW_HEIGHT - 50});
    reg.add_component<component::velocity>(floor_id, {-BACKGROUND_SPEED, 0});

    return floor_id;
}

entity Factory::make_wall()
{
    entity wall_id = reg.spawn_entity();

    auto &wall_sprite = reg.add_component<component::drawable>(wall_id, component::drawable());
    wall_sprite.setTextureFromName("assets/sprites/bakcground/wall.gif");

    reg.add_component<component::position>(wall_id, {0, 0});
    reg.add_component<component::velocity>(wall_id, {0, 0});

    return wall_id;
}

entity Factory::make_background()
{
    entity background_id = reg.spawn_entity();

    auto &background_sprite = reg.add_component<component::drawable>(background_id, component::drawable());
    background_sprite.setTextureFromName("background");
    background_sprite.sprite.setTextureRect(sf::IntRect(0, 0, 100000, 500));
    ResourceManager::Instance().getTexture("background").setRepeated(true);

    reg.add_component<component::position>(background_id, {0, 0});
    reg.add_component<component::velocity>(background_id, {-BACKGROUND_SPEED, 0});

    return background_id;
}

entity Factory::make_title()
{
    entity title_id = reg.spawn_entity();

    reg.add_component<component::position>(title_id, {100, 100});
    auto &title_text = reg.add_component<component::text>(title_id, component::text());
    title_text.setFontFromName("arcade");
    title_text.text.setString("R TYPE");
    title_text.text.setCharacterSize(60);
    title_text.text.setFillColor(sf::Color::White);
    
    return title_id;
}

entity Factory::make_start_text()
{
    entity text_id = reg.spawn_entity();

    reg.add_component<component::position>(text_id, {350, 150});
    auto &title_text = reg.add_component<component::text>(text_id, component::text());
    title_text.setFontFromName("arcade");
    title_text.text.setString("Press Space to start");
    title_text.text.setCharacterSize(16);
    title_text.text.setFillColor(sf::Color::White);
    reg.add_component<component::logic>(text_id, component::logic{start_text_logic});

    return text_id;
}

entity Factory::make_menu_background_music()
{
    entity music_id = reg.spawn_entity();
    auto &music = reg.add_component<component::audio>(music_id, component::audio());
    music.audio.reset(new sf::Music);
    music.audio->openFromFile("assets/audio/menu.mp3");
    music.audio->setLoop(true);
    music.audio->play();

    return music_id;
}

entity Factory::make_game_background_music()
{
    entity music_id = reg.spawn_entity();
    auto &music = reg.add_component<component::audio>(music_id, component::audio());
    music.audio.reset(new sf::Music);
    music.audio->openFromFile("assets/audio/incredible.mp3");
    music.audio->setLoop(true);
    music.audio->play();

    return music_id;
}

