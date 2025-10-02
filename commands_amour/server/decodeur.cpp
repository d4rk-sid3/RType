/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     decodeur                                                                         *
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

MoveResponse decodeMoveResponse(const std::vector<uint8_t>& buffer)
{
    MoveResponse pos;

    if (buffer[0] != 0x24) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.position.x = (buffer[5] << 8) |  buffer[6];
    pos.position.y = (buffer[7] << 8) |  buffer[8];
    pos.direction.x = (buffer[9] << 8) |  buffer[10];
    pos.direction.y = (buffer[11] << 8) |  buffer[12];
    pos.speed = buffer[13];
    pos.timestamp.milliseconds = (buffer[14] << 56) | (buffer[15] << 48) | (buffer[16] << 40)
            | (buffer[17] << 32) | (buffer[18] << 24) | (buffer[19] << 16) | (buffer[20] << 8) | buffer[21];

    return pos;
}

ShootResponse decodeShootResponse(const std::vector<uint8_t>& buffer)
{
    ShootResponse pos;

    if (buffer[0] != 0x26) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.bullet_position.x = (buffer[5] << 8) |  buffer[6];
    pos.bullet_position.y = (buffer[7] << 8) |  buffer[8];
    pos.bullet_direction.x = (buffer[9] << 8) |  buffer[10];
    pos.bullet_direction.y = (buffer[11] << 8) |  buffer[12];
    pos.bullet_speed = buffer[13];
    pos.bullet = static_cast<BULLET_TYPE>(buffer[14]);

    return pos;
}

PickupItemResponse decodePickupItemResponse(const std::vector<uint8_t>& buffer)
{
    PickupItemResponse pos;

    if (buffer[0] != 0x26) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.item_id = (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
    pos.item_position.x = (buffer[9] << 8) |  buffer[10];
    pos.item_position.y = (buffer[11] << 8) |  buffer[12];
    pos.timestamp.milliseconds = (buffer[13] << 56) | (buffer[14] << 48) | (buffer[15] << 40)
        | (buffer[16] << 32) | (buffer[17] << 24) | (buffer[18] << 16) | (buffer[19] << 8) | buffer[20];

    return pos;
}

MoveResponse updateMoveResponse(const MoveResponse& oldpos, const MoveResponse& pos)
{
    MoveResponse p = oldpos;

    p.player_id = pos.player_id;
    p.position = pos.position;
    p.direction = pos.direction;
    p.speed = pos.speed;
    p.timestamp = pos.timestamp;

    return p;
}

ShootResponse updateShootResponse(const ShootResponse& oldpos, const ShootResponse& pos)
{
    ShootResponse p = oldpos;

    p.player_id = pos.player_id;
    p.bullet_position = pos.bullet_position;
    p.bullet_direction = pos.bullet_direction;
    p.bullet_speed = pos.bullet_speed;
    p.bullet = pos.bullet;

    return p;
}

PickupItemResponse updatePickupItemResponse(const PickupItemResponse& oldpos, const PickupItemResponse& pos)
{
    PickupItemResponse p = oldpos;

    p.player_id = pos.player_id;
    p.item_id = pos.item_id;
    p.item_position = pos.item_position;
    p.timestamp = pos.timestamp;

    return p;
}
