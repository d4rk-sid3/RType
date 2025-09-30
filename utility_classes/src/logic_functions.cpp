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

#define PLAYER_MISSISLE_SPEED 10
#define ENEMY_MISSISLE_SPEED 5
#define SCROLL_SPEED 5
#define PLAYER_SPEED 250
#define TROOPER_SPEED_X 150
#define TROOPER_SPEED_Y 60
#define PLAYER_SHOOT_COOLDOWN 0.25

using namespace component;

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

        reg.kill_entity(en);
    }
}

void red_trooper_logic(double delta, registry &reg, entity en)
{
    static bool shot = false;
    static double t = 0.0;
    velocity &vel = reg.get_components<velocity>()[en].value();
    
    t += delta;
    vel.vx = -TROOPER_SPEED_X;
    vel.vy = sin(t*2) * TROOPER_SPEED_Y;
    
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
    
    const auto &player_pos = reg.get_components<position>()[player_entity_id].value();
    position &pos = reg.get_components<position>()[en].value();

    if (distance(player_pos.x, player_pos.y, pos.x, pos.y) < 200 && !shot) {
        shot = true;
        Factory fac(reg);
        entity missile = fac.make_enemy_missile();
        position &missile_pos = reg.get_components<position>()[missile].value();
        velocity &missile_vel = reg.get_components<velocity>()[missile].value();
        missile_vel.vx = player_pos.x - pos.x;
        missile_vel.vy = player_pos.y - pos.y;
        missile_pos.x = pos.x;
        missile_pos.y = pos.y;
    }
}
