/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     commands                                                                         *
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

#include "../include/commands.hpp"

std::vector<uint8_t> encodeMoveResponse(const MoveResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x24);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x & 0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    buffer.emplace_back((pos.direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.x & 0xFF);
    buffer.emplace_back((pos.direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.y & 0xFF);

    buffer.emplace_back((pos.speed >> 8) & 0xFF);
    buffer.emplace_back(pos.speed & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeShootResponse(const ShootResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x26);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.bullet_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_position.x & 0xFF);
    buffer.emplace_back((pos.bullet_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_position.y & 0xFF);

    buffer.emplace_back((pos.bullet_direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_direction.x & 0xFF);
    buffer.emplace_back((pos.bullet_direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_direction.y & 0xFF);

    buffer.emplace_back((pos.bullet_speed >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_speed & 0xFF);

    buffer.emplace_back(pos.bullet & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodePickupItemResponse(const PickupItemResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x26);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.item_id >> 24) & 0xFF);
    buffer.emplace_back((pos.item_id >> 16) & 0xFF);
    buffer.emplace_back((pos.item_id >> 8) & 0xFF);
    buffer.emplace_back(pos.item_id & 0xFF);

    buffer.emplace_back((pos.item_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.item_position.x & 0xFF);
    buffer.emplace_back((pos.item_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.item_position.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}
