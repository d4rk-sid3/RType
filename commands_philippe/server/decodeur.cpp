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

GameStartedResponse decodeGameStartedResponse(const std::vector<uint8_t>& buffer)
{
    GameStartedResponse pos;

    if (buffer[0] != 0x32) {
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

GamePausedResponse decodeGamePausedResponse(const std::vector<uint8_t>& buffer)
{
    GamePausedResponse pos;

    if (buffer[0] != 0x34) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[1]) << 56) | 
        (static_cast<uint64_t>(buffer[2]) << 48) | 
        (static_cast<uint64_t>(buffer[3]) << 40) | 
        (static_cast<uint64_t>(buffer[4]) << 32) |
        (static_cast<uint64_t>(buffer[5]) << 24) | 
        (static_cast<uint64_t>(buffer[6]) << 16) | 
        (static_cast<uint64_t>(buffer[7]) << 8)  | 
        static_cast<uint64_t>(buffer[8]);
    pos.current_state = static_cast<PauseState>(buffer[9]);
    return pos;
}

GameStateResponse decodeGameStateResponse(const std::vector<uint8_t>& buffer)
{
    GameStateResponse pos;
    int k = 2;

    if (buffer[0] != 0x35) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.num_disconnected = buffer[1];
    for (int i = 0; i < pos.num_disconnected; i++) {
        pos.ids.emplace_back((buffer[k] << 24) | (buffer[k+1] << 16) | (buffer[k+2] << 8) | buffer[k+3]);
        k +=4;
    }
    return pos;
}

EnemySpawnedResponse decodeEnemySpawnedResponse(const std::vector<uint8_t>& buffer)
{
    EnemySpawnedResponse pos;

    if (buffer[0] != 0x36) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.enemy_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.enemy_type = static_cast<EnemyType>(buffer[5]);
    pos.position.x = (buffer[6] << 8) | buffer[7];
    pos.position.y = (buffer[8] << 8) | buffer[9];
    pos.direction.x = (buffer[10] << 8) | buffer[11];
    pos.direction.y = (buffer[12] << 8) | buffer[13];
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[14]) << 56) | 
        (static_cast<uint64_t>(buffer[15]) << 48) | 
        (static_cast<uint64_t>(buffer[16]) << 40) | 
        (static_cast<uint64_t>(buffer[17]) << 32) |
        (static_cast<uint64_t>(buffer[18]) << 24) | 
        (static_cast<uint64_t>(buffer[19]) << 16) | 
        (static_cast<uint64_t>(buffer[20]) << 8)  | 
        static_cast<uint64_t>(buffer[21]);
    return pos;
}

EnemyMovedResponse decodeEnemyMovedResponse(const std::vector<uint8_t>& buffer)
{
    EnemyMovedResponse pos;

    if (buffer[0] != 0x37) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.enemy_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.enemy_type = static_cast<EnemyType>(buffer[5]);
    pos.position.x = (buffer[6] << 8) | buffer[7];
    pos.position.y = (buffer[8] << 8) | buffer[9];
    pos.direction.x = (buffer[10] << 8) | buffer[11];
    pos.direction.y = (buffer[12] << 8) | buffer[13];
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[14]) << 56) | 
        (static_cast<uint64_t>(buffer[15]) << 48) | 
        (static_cast<uint64_t>(buffer[16]) << 40) | 
        (static_cast<uint64_t>(buffer[17]) << 32) |
        (static_cast<uint64_t>(buffer[18]) << 24) | 
        (static_cast<uint64_t>(buffer[19]) << 16) | 
        (static_cast<uint64_t>(buffer[20]) << 8)  | 
        static_cast<uint64_t>(buffer[21]);
    return pos;
}

EnemyFiredResponse decodeEnemyFiredResponse(const std::vector<uint8_t>& buffer) {
    EnemyFiredResponse pos;

    if (buffer[0] != 0x38) {
        throw std::runtime_error("Invalid message type !");
    }
    pos.enemy_id = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    pos.enemy_type = static_cast<EnemyType>(buffer[5]);
    pos.position.x = (buffer[6] << 8) | buffer[7];
    pos.position.y = (buffer[8] << 8) | buffer[9];
    pos.direction.x = (buffer[10] << 8) | buffer[11];
    pos.direction.y = (buffer[12] << 8) | buffer[13];
    pos.timestamp.milliseconds = 
        (static_cast<uint64_t>(buffer[14]) << 56) | 
        (static_cast<uint64_t>(buffer[15]) << 48) | 
        (static_cast<uint64_t>(buffer[16]) << 40) | 
        (static_cast<uint64_t>(buffer[17]) << 32) |
        (static_cast<uint64_t>(buffer[18]) << 24) | 
        (static_cast<uint64_t>(buffer[19]) << 16) | 
        (static_cast<uint64_t>(buffer[20]) << 8)  | 
        static_cast<uint64_t>(buffer[21]);
    return pos;
}

