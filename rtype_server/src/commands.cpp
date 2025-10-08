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

std::vector<int16_t> Server::encodeEnemyMovedResponse(const EnemyMovedResponse& pos)
{
    std::vector <int16_t> buffer;

    buffer.emplace_back(0x37);
    buffer.emplace_back(pos.enemy_id);
    buffer.emplace_back(pos.enemy_type);
    buffer.emplace_back(pos.position.x);
    buffer.emplace_back(pos.position.y);
    return buffer;
}

std::vector<int16_t> Server::encodeNbrEntity(const NbrEntity& pos)
{
    std::vector <int16_t> buffer;

    buffer.emplace_back(0x38);

    buffer.emplace_back(pos.nbr);
    return buffer;
}
