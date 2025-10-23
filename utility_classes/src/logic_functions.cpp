/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * logic_functions *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

/**
 * @file logic_functions.cpp
 * @author Farouk OKANLA
 * @brief This file contains definitions of the logic functions
 * @version 0.1
 * @date 2025-10-13
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "logic_functions.hpp"

#include <cmath>

#include "../../rtype_server/include/server.hpp"
#include "Factory.hpp"
#include "entity.hpp"

using namespace component;

bool boss_dead = false;

bool boss1_dead = false;
bool boss2_dead = false;

bool final_boss_dead = false;

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void player_logic(double delta, registry& reg, entity en) {
    position& pos = reg.get_components<position>()[(size_t)en].value();
    const controllable& control =
        reg.get_components<controllable>()[(size_t)en].value();
    velocity& vel = reg.get_components<velocity>()[(size_t)en].value();
    static double shoot_timer = 0;

    shoot_timer += delta;

    if (control.left) {
        vel.vx = -PLAYER_SPEED;
    } else if (control.right) {
        vel.vx = PLAYER_SPEED;
    } else {
        vel.vx = 0;
    }

    if (control.up) {
        vel.vy = -PLAYER_SPEED;
    } else if (control.down) {
        vel.vy = PLAYER_SPEED;
    } else {
        vel.vy = 0;
    }

    if (control.space && shoot_timer > PLAYER_SHOOT_COOLDOWN) {
        Factory fac(reg);

        shoot_timer = 0;
        entity missile = fac.make_player_missile();

        position& player_pos =
            reg.get_components<position>()[(size_t)en].value();
        position& missile_pos = reg.get_components<position>()[missile].value();
        missile_pos.x = player_pos.x;
        missile_pos.y = player_pos.y;
    }

    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        name& name_ = reg.get_components<name>()[en].value();
        if (name_._name == "player1") {
            player1_entity_id = -1;
        } else if (name_._name == "player2") {
            player2_entity_id = -1;
        }
        reg.kill_entity(en);
    }
}

bool shoot_at_player(registry& reg, position enemy_pos, double attack_range) {
    position target_pos;

    double distance1 = 1000000;
    double distance2 = 1000000;
    if (player1_entity_id != -1) {
        position player1_pos =
            reg.get_components<position>()[player1_entity_id].value();
        distance1 =
            distance(player1_pos.x, player1_pos.y, enemy_pos.x, enemy_pos.y);
        if (distance1 > attack_range) {
            distance1 = 1000000;
        }
    }
    if (player2_entity_id != -1) {
        position player2_pos =
            reg.get_components<position>()[player2_entity_id].value();
        distance2 =
            distance(player2_pos.x, player2_pos.y, enemy_pos.x, enemy_pos.y);
        if (distance2 > attack_range) {
            distance2 = 1000000;
        }
    }

    if (distance1 == distance2) {
        return false;
    }

    if (distance1 < distance2) {
        if (distance1 < attack_range)
            target_pos =
                reg.get_components<position>()[player1_entity_id].value();
        else
            return false;
    } else if (distance2 < distance1) {
        if (distance2 < attack_range)
            target_pos =
                reg.get_components<position>()[player2_entity_id].value();
        else
            return false;
    }

    Factory fac(reg);
    entity missile = fac.make_enemy_missile();
    position& missile_pos = reg.get_components<position>()[missile].value();
    missile_pos.x = enemy_pos.x + 8;
    missile_pos.y = enemy_pos.y + 8;

    velocity& vel = reg.get_components<velocity>()[missile].value();
    vel.vx = target_pos.x - enemy_pos.x;
    vel.vy = target_pos.y - enemy_pos.y;
    return true;
}

void plane_logic(double delta, registry& reg, entity en) {
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }
}


void red_trooper_logic(double delta, registry& reg, entity en) {
    static bool shot = false;
    static double shoot_timer = 0;
    static double t = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();

    if (shot) {
        shoot_timer += delta;
        if (shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            shoot_timer = 0;
            shot = false;
        }
    } else {
        shoot_timer = 0;
    }

    t += delta;
    vel.vx = -TROOPER_SPEED_X;
    vel.vy = sin(t * 2) * TROOPER_SPEED_Y;

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!shot)
        if (shoot_at_player(reg, pos, 200))
            shot = true;
}

void start_text_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;

    t += delta;

    text& text_var = reg.get_components<text>()[en].value();
    text_var.text.setFillColor(sf::Color(255, 255, 255, abs(sin(t * 2) * 255)));
}

void walker_logic(double delta, registry& reg, entity en) {
    static bool walker_shot = false;
    static double walker_shoot_timer = 0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    if (walker_shot) {
        walker_shoot_timer += delta;
        if (walker_shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            walker_shoot_timer = 0;
            walker_shot = false;
        }
    } else {
        walker_shoot_timer = 0;
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!walker_shot) {
        if (shoot_at_player(reg, pos, 200))
            walker_shot = true;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
}

void fade_in_rect_logic(double delta, registry& reg, entity entity) {
    Factory fac(reg);
    static double t = 0.0;
    static double instancited = false;
    drawable& dr = reg.get_components<drawable>()[entity].value();

    t += delta;
    dr.sprite.setColor(sf::Color(0, 0, 0, (int)round(t * 128)));
    if (dr.sprite.getColor().a >= 255) {
        reg.kill_entity(entity);
    }
    if (dr.sprite.getColor().a >= 220 && !instancited) {
        instancited = true;
        fac.make_fade_out_rect();
    }
}

void fade_out_rect_logic(double delta, registry& reg, entity entity) {
    static double t = 0.0;
    drawable& dr = reg.get_components<drawable>()[entity].value();

    t += delta;
    dr.sprite.setColor(sf::Color(0, 0, 0, 255 - (int)round(t * 128)));
    if (dr.sprite.getColor().a <= 0) {
        reg.kill_entity(entity);
    }
}

void boss_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    vel.vy = sin(t1 * 2) * BOSS_SPEED;

    if (pos.x > 500) {
        vel.vx = -BOSS_SPEED;
    } else {
        vel.vx = 0;
    }

    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_enemy_missile();
        entity missile2 = fac.make_enemy_missile();
        entity missile3 = fac.make_enemy_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();
        position& pos2 = reg.get_components<position>()[missile2].value();
        position& pos3 = reg.get_components<position>()[missile3].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();
        velocity& vel2 = reg.get_components<velocity>()[missile2].value();
        velocity& vel3 = reg.get_components<velocity>()[missile3].value();

        pos1.x = pos.x + 30;
        pos2.x = pos.x + 30;
        pos3.x = pos.x + 30;
        pos1.y = pos.y + 15;
        pos2.y = pos.y + 15;
        pos3.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = -BOSS_MISSILE_SPEED / 2;
        vel2.vx = -BOSS_MISSILE_SPEED;
        vel2.vy = BOSS_MISSILE_SPEED / 2;
        vel3.vx = -BOSS_MISSILE_SPEED;
        vel3.vy = 0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        boss_dead = true;
        reg.kill_entity(en);
    }
}

void gtrooper_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static size_t target_player_id = player1_entity_id;
    static double retarget_timer = 0.0;
    const double RETARGET_INTERVAL = 3.0;
    const float TARGET_X = 500.0f;
    const float X_THRESHOLD = 10.0f;

    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();
    
    bool reached_target_x = (pos.x <= TARGET_X + X_THRESHOLD);
    
    if (!reached_target_x) {
        vel.vx = -BOSS_SPEED;
        vel.vy = 0;
        return;
    }
    
    vel.vx = 0;
    
    retarget_timer += delta;
    if (retarget_timer >= RETARGET_INTERVAL) {
        size_t target_player_id_ = rand() % 2;

        if (target_player_id_ == 0) {
            target_player_id = player1_entity_id;
        }
        if (target_player_id_ == 1) {
            target_player_id = player2_entity_id;
        }
        retarget_timer = 0.0;
    }
    
    auto& positions = reg.get_components<position>();

    if (!positions[target_player_id].has_value()) {
        vel.vy = 0;
        return;
    }
    
    position& player_pos = positions[target_player_id].value();
    
    float diff_y = player_pos.y - pos.y;
    const float ALIGN_THRESHOLD = 20.0f;
    
    if (fabs(diff_y) > ALIGN_THRESHOLD) {
        vel.vy = (diff_y > 0) ? BOSS_SPEED : -BOSS_SPEED;
    } else {
        vel.vy = 0;
    }
    
    bool is_aligned = (fabs(player_pos.y - pos.y) <= ALIGN_THRESHOLD);
    
    t += delta;
    if (is_aligned) {
        printf("Aligned. T: %f\n", t);
        if (t >= BOSS_SHOOT_COOLDOWN) {
            printf("Shooting\n");
            t = 0;
            Factory fac(reg);
            entity missile1 = fac.make_big_missile();
            
            position& missile_pos1 = reg.get_components<position>()[missile1].value();
            
            missile_pos1.x = pos.x + 30;
            missile_pos1.y = pos.y + 15;
        }
    }
    
    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
    
    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& explosion_pos = reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;
        boss_dead = true;
        reg.kill_entity(en);
    }
}

void force_logic(double delta, registry& reg, entity en)
{
    static int attached_to = -1;
    static double shoot_cooldown = 0.0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        position & pos = reg.get_components<position>()[en].value();
        double p1_distance = 100000;
        double p2_distance = 100000;

        if (player1_entity_id != -1) {
            position& p1_pos = reg.get_components<position>()[player1_entity_id].value();
            p1_distance = distance(p1_pos.x, p1_pos.y, pos.x, pos.y);
        }
        if (player2_entity_id != -1) {
            position& p2_pos = reg.get_components<position>()[player2_entity_id].value();
            p2_distance = distance(p2_pos.x, p2_pos.y, pos.x, pos.y);
        }
        if (p1_distance < p2_distance) {
            attached_to = player1_entity_id;
        } else if (p2_distance < p1_distance) {
            attached_to = player2_entity_id;
        } else {
            return;
        }

        hurtbox &new_hb = reg.add_component<component::hurtbox>(en, component::hurtbox());
        new_hb.health = 20;
        new_hb.group = 1;
        new_hb.width = 24;
        new_hb.height = 19;
    }

    if (attached_to != -1) {
        position & pos = reg.get_components<position>()[en].value();
        position & player_pos = reg.get_components<position>()[attached_to].value();
        pos.x = player_pos.x + 50;
        pos.y = player_pos.y;

        shoot_cooldown += delta;
        if (shoot_cooldown >= FORCE_SHOOT_COOLDOWN) {
            Factory fac(reg);
            entity missile1 = fac.make_player_missile();
            entity missile2 = fac.make_player_missile();
            position & pos1 = reg.get_components<position>()[missile1].value();
            position & pos2 = reg.get_components<position>()[missile2].value();
            pos1.x = player_pos.x + 55;
            pos1.y = player_pos.y - 15;
            pos2.x = player_pos.x + 55;
            pos2.y = player_pos.y + 15;
            shoot_cooldown = 0;
        }
    }
}

void tourelles_logic(double delta, registry& reg, entity en) {
    static bool tourelles_shot = false;
    static double tourelles_shoot_timer = 0;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& pos = reg.get_components<position>()[en].value();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        reg.kill_entity(en);
    }

    if (tourelles_shot) {
        tourelles_shoot_timer += delta;
        if (tourelles_shoot_timer > ENEMY_SHOOT_COOLDOWN) {
            tourelles_shoot_timer = 0;
            tourelles_shot = false;
        }
    } else {
        tourelles_shoot_timer = 0;
    }

    position& pos = reg.get_components<position>()[en].value();
    if (!tourelles_shot) {
        if (shoot_at_player(reg, pos, 200))
            tourelles_shot = true;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& pos = reg.get_components<position>()[en].value();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
}

void small_shooter_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x > 500 && !ready) {
        vel.vx = -BOSS_SPEED;
    } else {
        ready = true;
    }

    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = sin(t1 * 2) * BOSS_SPEED;


    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_enemy_missile();
        entity missile2 = fac.make_enemy_missile();
        entity missile3 = fac.make_enemy_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();
        position& pos2 = reg.get_components<position>()[missile2].value();
        position& pos3 = reg.get_components<position>()[missile3].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();
        velocity& vel2 = reg.get_components<velocity>()[missile2].value();
        velocity& vel3 = reg.get_components<velocity>()[missile3].value();

        pos1.x = pos.x + 30;
        pos2.x = pos.x + 30;
        pos3.x = pos.x + 30;
        pos1.y = pos.y + 15;
        pos2.y = pos.y + 15;
        pos3.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = -BOSS_MISSILE_SPEED / 2;
        vel2.vx = -BOSS_MISSILE_SPEED;
        vel2.vy = BOSS_MISSILE_SPEED / 2;
        vel3.vx = -BOSS_MISSILE_SPEED;
        vel3.vy = 0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        boss1_dead = true;
        reg.kill_entity(en);
    }
}

void big_shooter_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x > 500 && !ready) {
        vel.vx = -BOSS_SPEED;
    } else {
        ready = true;
    }

    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = sin(t1 * 2) * BOSS_SPEED;


    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_big_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = 0.0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        boss1_dead = true;
        reg.kill_entity(en);
    }
}

void final_boss_logic(double delta, registry& reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    static double t2 = 0.0;
    static bool ready = false;
    hurtbox& hb = reg.get_components<hurtbox>()[en].value();
    velocity& vel = reg.get_components<velocity>()[en].value();
    position& pos = reg.get_components<position>()[en].value();

    t1 += delta;

    if (pos.x > 500 && !ready) {
        vel.vx = -BOSS_SPEED;
    } else {
        ready = true;
    }

    vel.vy = sin(t1 * 2) * BOSS_SPEED;
    vel.vx = 0;

    if (t < BOSS_SHOOT_COOLDOWN) {
        t += delta;
    } else {
        t = 0;
        Factory fac(reg);
        entity missile1 = fac.make_big_missile();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[missile1].value();

        velocity& vel1 = reg.get_components<velocity>()[missile1].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED;
        vel1.vy = 0.0;
    }
    if (t2 < BOSS_SPAWN_COOLDOWN) {
        t2 += delta;
    } else {
        t2 = 0;
        Factory fac(reg);
        entity plane = fac.make_plane();

        position& pos = reg.get_components<position>()[en].value();
        position& pos1 = reg.get_components<position>()[plane].value();

        pos1.x = pos.x + 30;
        pos1.y = pos.y + 15;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position& hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position& explosion_pos =
            reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        boss1_dead = true;
        reg.kill_entity(en);
    }
}
