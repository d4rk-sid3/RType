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

#include "../include/server.hpp"

std::vector<int8_t> Server::encodeEnemyMovedResponse(const EnemyMovedResponse& pos)
{
    std::vector <int8_t> buffer;

    buffer.emplace_back(0x37);

    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back((pos.enemy_type >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);

    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    return buffer;
}

std::vector<int8_t> Server::encodeNbrEntity(const NbrEntity& pos)
{
    std::vector <int8_t> buffer;

    buffer.emplace_back(0x38);

    buffer.emplace_back((pos.nbr >> 8) & 0xFF);
    buffer.emplace_back(pos.nbr & 0xFF);
    return buffer;
}
