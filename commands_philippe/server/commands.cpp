#include "../include/commands.hpp"

std::vector<uint8_t> encodePlayerStateResponse(const PlayerStateResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x28);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.remaining_health >> 16) & 0xFF);
    buffer.emplace_back(pos.remaining_health & 0xFF);

    buffer.emplace_back((pos.score >> 24) & 0xFF);
    buffer.emplace_back((pos.score >> 16) & 0xFF);
    buffer.emplace_back((pos.score >> 8) & 0xFF);
    buffer.emplace_back(pos.score & 0xFF);

    buffer.emplace_back(pos.current_level & 0xFF);
    
    buffer.emplace_back(pos.state & 0xFF);

    return buffer;
}