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

MoveResponse Server::decodeMoveResponse(const std::vector<int8_t>& buffer)
{
    MoveResponse pos;

    int16_t type = (buffer[0] << 8) | buffer[1];

    if (type != 0x24) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.type = (buffer[0] << 8) | buffer[1];
    pos.player_id = (buffer[2] << 8) | buffer[3];
    pos.direction = static_cast<Direction>((buffer[4] << 8) | buffer[5]);

    return pos;
}