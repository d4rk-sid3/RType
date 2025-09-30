#include "../include/commands.hpp"

PlayerStateResponse decodePlayerStateResponse(const std::vector<uint8_t>& buffer)
{
    PlayerStateResponse pos;

    if (buffer[0] != 0x28) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.remaining_health = (buffer[5] << 8) | buffer[6];
    pos.score = pos.player_id = (buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10]; 
    pos.current_level = buffer[11];
    pos.state = static_cast<State>(buffer[12]);
    return pos;
}
