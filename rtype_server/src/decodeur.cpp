/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     decodeur                                                                         *
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

#include "../include/server.hpp"

// MoveRequest Server::decodeMoveRequest(const std::vector<int8_t>& buffer)
// {
//     MoveRequest pos;

//     if (buffer[0] != 0x23) {
//         throw std::runtime_error("Type de message invalide !");
//     }

//     pos.type = buffer[0];
//     pos.direction = static_cast<Direction>(buffer[1]);

//     return pos;
// }

// ShootRequest Server::decodeShootRequest(const std::vector<int8_t>& buffer)
// {

//     ShootRequest pos;

//     if (buffer[0] != 0x25) {
//         throw std::runtime_error("Type de message invalide !");
//     }

//     pos.type = buffer[0];

//     return pos;
// }

// PickupItemResquest Server::decodePickupItemResquest(const std::vector<int8_t>& buffer)
// {

//     PickupItemResquest pos;

//     if (buffer[0] != 0x40) {
//         throw std::runtime_error("Type de message invalide !");
//     }

//     pos.type = buffer[0];

//     return pos;
// }

// GamePausedRequest Server::decodeGamePausedRequest(const std::vector<int8_t>& buffer)
// {
//     GamePausedRequest pos;

//     if (buffer[0] != 0x33) {
//         throw std::runtime_error("Invalid message type !");
//     }

//     pos.type = buffer[0];

//     return pos;
// }

// MoveResponse Server::updateMoveResponse(const MoveResponse& oldpos, const MoveResponse& pos)
// {
//     MoveResponse p = oldpos;

//     p.player_id = pos.player_id;
//     p.position.x = pos.position.x;
//     p.position.y = pos.position.y;
//     p.direction.x = pos.direction.x;
//     p.direction.y = pos.direction.y;
//     p.speed = pos.speed;
//     p.timestamp = pos.timestamp;

//     return p;
// }

// ShootResponse Server::updateShootResponse(const ShootResponse& oldpos, const ShootResponse& pos)
// {
//     ShootResponse p = oldpos;

//     p.player_id = pos.player_id;
//     p.bullet_position.x = pos.bullet_position.x;
//     p.bullet_position.y = pos.bullet_position.y;
//     p.bullet_direction.x = pos.bullet_direction.x;
//     p.bullet_direction.y = pos.bullet_direction.y;
//     p.bullet_speed = pos.bullet_speed;
//     p.bullet = pos.bullet;

//     return p;
// }

// PickupItemResponse Server::updatePickupItemResponse(const PickupItemResponse& oldpos, const PickupItemResponse& pos)
// {
//     PickupItemResponse p = oldpos;

//     p.player_id = pos.player_id;
//     p.item_id = pos.item_id;
//     p.item_position.x = pos.item_position.x;
//     p.item_position.y = pos.item_position.y;
//     p.timestamp = pos.timestamp;

//     return p;
// }
