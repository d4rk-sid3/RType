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

    if (buffer.size() < 9) {
        throw std::runtime_error("Not enough data in buffer to decode EnemyMovedResponse");
    }

    auto toInt16 = [](int8_t high, int8_t low) -> int16_t {
        return static_cast<int16_t>(
            (static_cast<uint8_t>(high) << 8) | static_cast<uint8_t>(low)
        );
    };
    
    pos.type = buffer[0];
    pos.enemy_id = toInt16(buffer[1], buffer[2]);
    pos.enemy_type = static_cast<EnemyType>(toInt16(buffer[3], buffer[4]));
    pos.position.x = toInt16(buffer[5], buffer[6]);
    pos.position.y = toInt16(buffer[7], buffer[8]);

    std::cout << "Enemy_Type: " << static_cast<int>(pos.enemy_type) << " ";
    std::cout << "Enemy_Pos_x: " << pos.position.x << " ";
    std::cout << "Enemy_Pos_y: " << pos.position.y << std::endl;

    buffer.erase(buffer.begin(), buffer.begin() + 9);
    return pos;
}

NbrEntity Client::decodeNbrEntity(std::vector<int8_t>& buffer)
{
    NbrEntity pos;

    if (buffer[0] != 0x38) {
        throw std::runtime_error("Invalid message type 0x38 !");
    }

    pos.type = buffer[0];
    pos.nbr = (buffer[1] << 8) | buffer[2];

    buffer.erase(buffer.begin(), buffer.begin() + 3);
    return pos;
}
