
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

std::vector<uint8_t> encodePlayerGameStateResponse(const PlayerGameStateResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x29);

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

    buffer.emplace_back(pos.game_state & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeBeatBossResponse(const BeatBossResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x30);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.boss_id >> 24) & 0xFF);
    buffer.emplace_back((pos.boss_id >> 16) & 0xFF);
    buffer.emplace_back((pos.boss_id >> 8) & 0xFF);
    buffer.emplace_back(pos.boss_id & 0xFF);

    buffer.emplace_back((pos.player_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.x & 0xFF);
    buffer.emplace_back((pos.player_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    buffer.emplace_back((pos.boss_state));

    return buffer;
}

std::vector<uint8_t> encodeCheckpointResponse(const CheckpointResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x31);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.checkpoint_id >> 24) & 0xFF);
    buffer.emplace_back((pos.checkpoint_id >> 16) & 0xFF);
    buffer.emplace_back((pos.checkpoint_id >> 8) & 0xFF);
    buffer.emplace_back(pos.checkpoint_id & 0xFF);

    buffer.emplace_back((pos.player_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.x & 0xFF);
    buffer.emplace_back((pos.player_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeGameStartedResponse(const GameStartedResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x32);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.checkpoint_id >> 24) & 0xFF);
    buffer.emplace_back((pos.checkpoint_id >> 16) & 0xFF);
    buffer.emplace_back((pos.checkpoint_id >> 8) & 0xFF);
    buffer.emplace_back(pos.checkpoint_id & 0xFF);

    buffer.emplace_back((pos.player_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.x & 0xFF);
    buffer.emplace_back((pos.player_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.player_position.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeGamePausedResponse(const GamePausedResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x34);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    buffer.emplace_back(pos.current_state & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeGameStateResponse(const GameStateResponse& pos, const std::vector<uint8_t> ids) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x35);

    /// 
    buffer.emplace_back(pos.num_disconnected & 0xFF);
    for (const auto& id: ids) {
        buffer.emplace_back((id >> 24) & 0xFF);
        buffer.emplace_back((id >> 16) & 0xFF);
        buffer.emplace_back((id >> 8) & 0xFF);
        buffer.emplace_back(id & 0xFF);
    }
    return buffer;
}

std::vector<uint8_t> encodeEnemySpawnedResponse(const EnemySpawnedResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x36);

    buffer.emplace_back((pos.enemy_id >> 24) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 16) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x & 0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    buffer.emplace_back((pos.direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.x & 0xFF);
    buffer.emplace_back((pos.direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos) {
    std::vector <uint8_t> buffer;

    buffer.emplace_back(0x37);

    buffer.emplace_back((pos.enemy_id >> 24) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 16) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);
    
    buffer.emplace_back((pos.direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.x & 0xFF);
    buffer.emplace_back((pos.direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodedEnemyFiredResponse(const EnemyFiredResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x38);

    buffer.emplace_back((pos.enemy_id >> 24) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 16) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);
    
    buffer.emplace_back((pos.direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.x & 0xFF);
    buffer.emplace_back((pos.direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeEnemyDiedResponse(const EnemyDiedResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x39);

    buffer.emplace_back((pos.enemy_id >> 24) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 16) & 0xFF);
    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}

std::vector<uint8_t> encodeCollisionResponse(const CollisionResponse& pos) {
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x40);

    buffer.emplace_back((pos.entity_id_1 >> 24) & 0xFF);
    buffer.emplace_back((pos.entity_id_1 >> 16) & 0xFF);
    buffer.emplace_back((pos.entity_id_1 >> 8) & 0xFF);
    buffer.emplace_back(pos.entity_id_1 & 0xFF);

    buffer.emplace_back((pos.entity_id_2 >> 24) & 0xFF);
    buffer.emplace_back((pos.entity_id_2 >> 16) & 0xFF);
    buffer.emplace_back((pos.entity_id_2 >> 8) & 0xFF);
    buffer.emplace_back(pos.entity_id_2 & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    buffer.emplace_back(pos.collision_type & 0xFF);

    buffer.emplace_back((pos.timestamp.milliseconds >> 56) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 48) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 40) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 32) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 24) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 16) & 0xFF);
    buffer.emplace_back((pos.timestamp.milliseconds >> 8) & 0xFF);
    buffer.emplace_back(pos.timestamp.milliseconds & 0xFF);

    return buffer;
}