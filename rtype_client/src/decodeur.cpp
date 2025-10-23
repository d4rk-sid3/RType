/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025 * Title           -
 * G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     - * decodeur *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░ * ▒
 * ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓ * █
 * ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █ *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#include "../include/client.hpp"

EnemyMovedResponse Client::decodeEnemyMovedResponse(std::vector<int8_t>& buffer
) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);

        tmp.insert(tmp.begin(), buffer.begin(), buffer.end() + 9);
    }

    if (tmp[0] != 0x37) {
        throw std::runtime_error("Invalid message type !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);

        buffer.erase(buffer.begin(), buffer.begin() + 9);
    }

    auto toInt16 = [](int8_t high, int8_t low) -> int16_t {
        return static_cast<int16_t>(
            (static_cast<uint8_t>(high) << 8) | static_cast<uint8_t>(low)
        );
    };

    EnemyMovedResponse pos;

    pos.type = tmp[0];
    pos.enemy_id = toInt16(tmp[1], tmp[2]);
    pos.enemy_type = static_cast<EnemyType>(toInt16(tmp[3], tmp[4]));
    pos.position.x = toInt16(tmp[5], tmp[6]);
    pos.position.y = toInt16(tmp[7], tmp[8]);

    std::cout << "Enemy_Type: " << static_cast<int>(pos.enemy_type) << " ";
    std::cout << "Enemy_Pos_x: " << pos.position.x << " ";
    std::cout << "Enemy_Pos_y: " << pos.position.y << std::endl;

    return pos;
}

GameState Client::decodeGameState(std::vector<int8_t>& buffer) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);

        tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 3);
    }

    if (tmp[0] != 0x40) {
        throw std::runtime_error("Invalid message type 0x38 !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);

        buffer.erase(buffer.begin(), buffer.begin() + 3);
    }

    GameState pos;

    pos.type = tmp[0];
    pos.gState = static_cast<GAMESTATE>((tmp[1] << 8) | tmp[2]);

    return pos;
}

NbrEntity Client::decodeNbrEntity(std::vector<int8_t>& buffer) {
    std::vector<int8_t> tmp;

    {
        std::lock_guard<std::mutex> lock(mtx);

        tmp.insert(tmp.begin(), buffer.begin(), buffer.begin() + 3);
    }

    if (tmp[0] != 0x38) {
        throw std::runtime_error("Invalid message type 0x38 !");
    }

    {
        std::lock_guard<std::mutex> lock(mtx);

        buffer.erase(buffer.begin(), buffer.begin() + 3);
    }

    NbrEntity pos;

    pos.type = tmp[0];
    pos.nbr = (tmp[1] << 8) | tmp[2];

    return pos;
}
