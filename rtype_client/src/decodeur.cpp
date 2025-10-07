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

#include "../include/client.hpp"

EnemyMovedResponse Client::decodeEnemyMovedResponse(std::vector<int8_t>& buffer)
{
    EnemyMovedResponse pos;

    if (buffer[0] != 0x37) {
        throw std::runtime_error("Invalid message type !");
    }

    if (buffer.size() < 8) {
        throw std::runtime_error("Not enough data in buffer to decode EnemyMovedResponse");
    }

    pos.type = buffer[0];
    pos.enemy_id = static_cast<int16_t>((static_cast<uint8_t>(buffer[1]) << 8) | static_cast<uint8_t>(buffer[2]));
    pos.enemy_type = static_cast<EnemyType>(buffer[3]);
    pos.position.x = static_cast<int16_t>((static_cast<uint8_t>(buffer[4]) << 8) | static_cast<uint8_t>(buffer[5]));
    pos.position.y = static_cast<int16_t>((static_cast<uint8_t>(buffer[6]) << 8) | static_cast<uint8_t>(buffer[7]));


    buffer.erase(buffer.begin(), buffer.begin() + 8);
    return pos;
}

NbrEntity Client::decodeNbrEntity(std::vector<int8_t>& buffer)
{
    NbrEntity pos;

    if (buffer[0] != 0x38) {
        throw std::runtime_error("Invalid message type !");
    }

    pos.type = buffer[0];
    pos.nbr = buffer[1];

    buffer.erase(buffer.begin(), buffer.begin() + 2);
    return pos;
}
