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

PlayerGameStateResponse decodePlayerGameStateResponse(const std::vector<uint8_t>& buffer)
{
    PlayerGameStateResponse pos;

    if (buffer[0] != 0x29) {
        throw std::runtime_error("Type de message invalide !");
    }

    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.remaining_health = (buffer[5] << 8) | buffer[6];
    pos.score = (buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10];
    pos.current_level = buffer[11];
    pos.state = static_cast<State>(buffer[12]);
    pos.game_state = static_cast<GameState>(buffer[13]);
    
    return pos;
}

BeatBossResponse decodeBeatBossResponse(const std::vector<uint8_t>& buffer)
{
    BeatBossResponse pos;

    if (buffer[0] != 0x30) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.boss_id = (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
    pos.player_position.x = (buffer[9] << 8) | buffer[10];
    pos.player_position.y = (buffer[11] << 8) | buffer[12];
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[13]) << 56) | 
        (static_cast<uint64_t>(buffer[14]) << 48) | 
        (static_cast<uint64_t>(buffer[15]) << 40) | 
        (static_cast<uint64_t>(buffer[16]) << 32) |
        (static_cast<uint64_t>(buffer[17]) << 24) | 
        (static_cast<uint64_t>(buffer[18]) << 16) | 
        (static_cast<uint64_t>(buffer[19]) << 8)  | 
        static_cast<uint64_t>(buffer[20]);
    pos.boss_state = static_cast<BossState>(buffer[21]);
    return pos;
}

CheckpointResponse decodeCheckpointResponse(const std::vector<uint8_t>& buffer)
{
    CheckpointResponse pos;

    if (buffer[0] != 0x31) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.player_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.checkpoint_id = (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
    pos.player_position.x = (buffer[9] << 8) | buffer[10];
    pos.player_position.y = (buffer[11] << 8) | buffer[12];
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[13]) << 56) | 
        (static_cast<uint64_t>(buffer[14]) << 48) | 
        (static_cast<uint64_t>(buffer[15]) << 40) | 
        (static_cast<uint64_t>(buffer[16]) << 32) |
        (static_cast<uint64_t>(buffer[17]) << 24) | 
        (static_cast<uint64_t>(buffer[18]) << 16) | 
        (static_cast<uint64_t>(buffer[19]) << 8)  | 
        static_cast<uint64_t>(buffer[20]);
        return pos;
    
}

