/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     logic_functions                                                                  *
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

#include "logic_functions.hpp"
#include "Factory.hpp"
#include "../../rtype_server/include/server.hpp"
#include <cmath>

using namespace component;

bool boss_dead = false;

double distance(double x1, double y1, double x2, double y2) { return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); }

void player_logic(double delta, registry &reg, entity en)
{
    position &pos = reg.get_components<position>()[(size_t)en].value();
    const controllable &control = reg.get_components<controllable>()[(size_t)en].value();
    velocity &vel = reg.get_components<velocity>()[(size_t)en].value();
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
        
        position &player_pos = reg.get_components<position>()[(size_t)en].value();
        position &missile_pos = reg.get_components<position>()[missile].value();
        missile_pos.x = player_pos.x;
        missile_pos.y = player_pos.y;
    }

    hurtbox &hb = reg.get_components<hurtbox>()[en].value();
    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position &pos = reg.get_components<position>()[en].value();
        position &explosion_pos = reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;

        name &name_ = reg.get_components<name>()[en].value();
        if (name_._name == "player1") {
            player1_entity_id = -1;
        } else if (name_._name == "player2") {
            player2_entity_id = -1;
        }
        reg.kill_entity(en);
    }
}

bool shoot_at_player(registry &reg, position enemy_pos, double attack_range)
{
    position target_pos;

    double distance1 = 1000000;
    double distance2 = 1000000;
    if (player1_entity_id != -1) {
        position player1_pos = reg.get_components<position>()[player1_entity_id].value();
        distance1 = distance(player1_pos.x, player1_pos.y, enemy_pos.x, enemy_pos.y);
        if (distance1 > attack_range) {
            distance1 = 1000000;
        }
    }
    if (player2_entity_id != -1) {
        position player2_pos = reg.get_components<position>()[player2_entity_id].value();
        distance2 = distance(player2_pos.x, player2_pos.y, enemy_pos.x, enemy_pos.y);
        if (distance2 > attack_range) {
            distance2 = 1000000;
        }
    }

    if (distance1 == distance2) {
        return false;
    }

    if (distance1 < distance2) {
        if (distance1 < attack_range)
            target_pos = reg.get_components<position>()[player1_entity_id].value();
        else
            return false;
    } else if (distance2 < distance1) {
        if (distance2 < attack_range)
            target_pos = reg.get_components<position>()[player2_entity_id].value();
        else
            return false;
    }
    
    Factory fac(reg);
    entity missile = fac.make_enemy_missile();
    position &missile_pos = reg.get_components<position>()[missile].value();
    missile_pos.x = enemy_pos.x + 8;
    missile_pos.y = enemy_pos.y + 8;

    velocity &vel = reg.get_components<velocity>()[missile].value();
    vel.vx = target_pos.x - enemy_pos.x;
    vel.vy = target_pos.y - enemy_pos.y;
    return true;
}

void red_trooper_logic(double delta, registry &reg, entity en)
{
    static bool shot = false;
    static double shoot_timer = 0;
    static double t = 0.0;
    hurtbox &hb = reg.get_components<hurtbox>()[en].value();
    velocity &vel = reg.get_components<velocity>()[en].value();

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
    vel.vy = sin(t*2) * TROOPER_SPEED_Y;

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position &pos = reg.get_components<position>()[en].value();
        position &hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position &pos = reg.get_components<position>()[en].value();
        position &explosion_pos = reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;
        
        reg.kill_entity(en);
    }
    
    
    position &pos = reg.get_components<position>()[en].value();
    if (!shot)
        if (shoot_at_player(reg, pos, 200))
            shot = true;
}

void start_text_logic(double delta, registry &reg, entity en)
{
    static double t = 0.0;

    t += delta;

    text &text_var = reg.get_components<text>()[en].value();
    text_var.text.setFillColor(sf::Color(255, 255, 255, abs(sin(t*2) * 255)));
}

void walker_logic(double delta, registry &reg, entity en)
{
    static bool walker_shot = false;
    static double walker_shoot_timer = 0;
    hurtbox &hb = reg.get_components<hurtbox>()[en].value();

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position &pos = reg.get_components<position>()[en].value();
        position &explosion_pos = reg.get_components<position>()[explosion].value();
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

    position &pos = reg.get_components<position>()[en].value();
    if (!walker_shot) {
        if (shoot_at_player(reg, pos, 200))
            walker_shot = true;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position &pos = reg.get_components<position>()[en].value();
        position &hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }
}

void fade_in_rect_logic(double delta, registry &reg, entity entity)
{
    Factory fac(reg);
    static double t = 0.0;
    static double instancited = false;
    drawable &dr = reg.get_components<drawable>()[entity].value();
    
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

void fade_out_rect_logic(double delta, registry &reg, entity entity)
{
    static double t = 0.0;
    drawable &dr = reg.get_components<drawable>()[entity].value();
    
    t += delta;
    dr.sprite.setColor(sf::Color(0, 0, 0, 255 - (int)round(t * 128)));
    if (dr.sprite.getColor().a <= 0) {
        reg.kill_entity(entity);
    }
}

void boss_logic(double delta, registry &reg, entity en) {
    static double t = 0.0;
    static double t1 = 0.0;
    hurtbox &hb = reg.get_components<hurtbox>()[en].value();
    velocity &vel = reg.get_components<velocity>()[en].value();
    position &pos = reg.get_components<position>()[en].value();

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
    
        position &pos = reg.get_components<position>()[en].value();
        position &pos1 = reg.get_components<position>()[missile1].value();
        position &pos2 = reg.get_components<position>()[missile2].value();
        position &pos3 = reg.get_components<position>()[missile3].value();
    
        velocity &vel1 = reg.get_components<velocity>()[missile1].value();
        velocity &vel2 = reg.get_components<velocity>()[missile2].value();
        velocity &vel3 = reg.get_components<velocity>()[missile3].value();

        pos1.x = pos.x + 30;
        pos2.x = pos.x + 30;
        pos3.x = pos.x + 30;
        pos1.y = pos.y + 15;
        pos2.y = pos.y + 15;
        pos3.y = pos.y + 15;

        vel1.vx = -BOSS_MISSILE_SPEED / 2;
        vel1.vy = -BOSS_MISSILE_SPEED;
        vel2.vx = -BOSS_MISSILE_SPEED / 2;
        vel2.vy = BOSS_MISSILE_SPEED;
        vel3.vx = -BOSS_MISSILE_SPEED;
        vel3.vy = 0;
    }

    if (hb.hurt) {
        Factory fac(reg);
        entity hit_effect = fac.make_hit_effect();
        position &hit_pos = reg.get_components<position>()[hit_effect].value();
        hit_pos.x = pos.x;
        hit_pos.y = pos.y;
    }

    if (hb.health <= 0) {
        Factory fac(reg);
        entity explosion = fac.make_explosion();
        position &explosion_pos = reg.get_components<position>()[explosion].value();
        explosion_pos.x = pos.x;
        explosion_pos.y = pos.y;
        
        boss_dead = true;
        reg.kill_entity(en);
    }
}
