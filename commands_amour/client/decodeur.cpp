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

#include "../include/commands.hpp"

MoveRequest decodeMoveRequest(const std::vector<uint8_t>& buffer)
{
    MoveRequest pos;

    if (buffer[0] != 0x23) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.direction = static_cast<Direction>(buffer[1]);

    return pos;
}

void decodeShootRequest(const std::vector<uint8_t>& buffer)
{

    if (buffer[0] != 0x25) {
        throw std::runtime_error("Type de message invalide !");
    }
}

void decodePickupItemRequest(const std::vector<uint8_t>& buffer)
{

    if (buffer[0] != 0x25) {
        throw std::runtime_error("Type de message invalide !");
    }
}
