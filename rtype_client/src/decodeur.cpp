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

EnemyMovedResponse Client::decodeEnemyMovedResponse(std::vector<int16_t>& buffer)
{
    EnemyMovedResponse pos;

    if (buffer[0] != 0x37) {
        throw std::runtime_error("Invalid message type !");
    }

    if (buffer.size() < 8) {
        throw std::runtime_error("Not enough data in buffer to decode EnemyMovedResponse");
    }

    pos.type = buffer[0];
    pos.enemy_id = buffer[1];
    pos.enemy_type = static_cast<EnemyType>(buffer[2]);
    pos.position.x = buffer[3];
    pos.position.y = buffer[4];

    std::cout << "Enemy_Type: "  << static_cast<int>(pos.enemy_type) << " ";
    std::cout << "Enemy_Pos_x: "  << static_cast<double>(pos.position.x) << " ";
    std::cout << "Enemy_Pos_y: "  << static_cast<double>(pos.position.y) << std::endl;

    buffer.erase(buffer.begin(), buffer.begin() + 5);
    return pos;
}

NbrEntity Client::decodeNbrEntity(std::vector<int16_t>& buffer)
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
