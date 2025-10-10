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

MoveResponse Server::decodeMoveResponse(std::vector<int8_t>& buffer)
{
    MoveResponse pos;

    {
        std::lock_guard<std::mutex> lock(mtx);

        if (buffer[0] != 0x24) {
            throw std::runtime_error("Type de message invalide !");
        }
    
        pos.type = buffer[0];
        pos.player_id = (buffer[1] << 8) | buffer[2];
        pos.direction = static_cast<Direction>((buffer[3] << 8) | buffer[4]);
    
        buffer.clear();
    }

    return pos;
}

ActionResponse Server::decodeActionResponse(std::vector<int8_t>& buffer)
{
    ActionResponse pos;

    {
        std::lock_guard<std::mutex> lock(mtx);

        if (buffer[0] != 0x25) {
            throw std::runtime_error("Type de message invalide !");
        }
    
        pos.type = buffer[0];
        pos.player_id = (buffer[1] << 8) | buffer[2];
        pos.input = static_cast<Action>((buffer[3] << 8) | buffer[4]);
    
        buffer.clear();
    }

    return pos;
}
