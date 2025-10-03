/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-8                                          *
 * Description     -                                                                    *
 *     commands                                                                         *
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

#include "../include/server.hpp"

std::vector<uint8_t> Server::encodeEnemyMovedResponse(const EnemyMovedResponse& pos)
{
    std::vector <uint8_t> buffer;

    buffer.emplace_back(0x37);

    buffer.emplace_back((pos.enemy_id >> 8) & 0xFF);
    buffer.emplace_back(pos.enemy_id & 0xFF);

    buffer.emplace_back(pos.enemy_type & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x &0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);
    
    return buffer;
}

std::vector<uint8_t> Server::encodeNbrEntity(const NbrEntity& pos)
{
    std::vector <uint8_t> buffer;

    buffer.emplace_back(0x38);

    buffer.emplace_back(pos.nbr);
    return buffer;
}




std::vector<uint8_t> Server::encodeMoveResponse(const MoveResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x24);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.position.x & 0xFF);
    buffer.emplace_back((pos.position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.position.y & 0xFF);

    buffer.emplace_back((pos.direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.x & 0xFF);
    buffer.emplace_back((pos.direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.direction.y & 0xFF);

    buffer.emplace_back(pos.speed);

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

std::vector<uint8_t> Server::encodeShootResponse(const ShootResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x26);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.bullet_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_position.x & 0xFF);
    buffer.emplace_back((pos.bullet_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_position.y & 0xFF);

    buffer.emplace_back((pos.bullet_direction.x >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_direction.x & 0xFF);
    buffer.emplace_back((pos.bullet_direction.y >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_direction.y & 0xFF);

    buffer.emplace_back((pos.bullet_speed >> 8) & 0xFF);
    buffer.emplace_back(pos.bullet_speed & 0xFF);

    buffer.emplace_back(pos.bullet & 0xFF);

    return buffer;
}

std::vector<uint8_t> Server::encodePickupItemResponse(const PickupItemResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x26);

    buffer.emplace_back((pos.player_id >> 24) & 0xFF);
    buffer.emplace_back((pos.player_id >> 16) & 0xFF);
    buffer.emplace_back((pos.player_id >> 8) & 0xFF);
    buffer.emplace_back(pos.player_id & 0xFF);

    buffer.emplace_back((pos.item_id >> 24) & 0xFF);
    buffer.emplace_back((pos.item_id >> 16) & 0xFF);
    buffer.emplace_back((pos.item_id >> 8) & 0xFF);
    buffer.emplace_back(pos.item_id & 0xFF);

    buffer.emplace_back((pos.item_position.x >> 8) & 0xFF);
    buffer.emplace_back(pos.item_position.x & 0xFF);
    buffer.emplace_back((pos.item_position.y >> 8) & 0xFF);
    buffer.emplace_back(pos.item_position.y & 0xFF);

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

std::vector<uint8_t> Server::encodePlayerStateResponse(const PlayerStateResponse& pos)
{
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

std::vector<uint8_t> Server::encodePlayerGameStateResponse(const PlayerGameStateResponse& pos)
{
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

std::vector<uint8_t> Server::encodeBeatBossResponse(const BeatBossResponse& pos)
{
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

std::vector<uint8_t> Server::encodeCheckpointResponse(const CheckpointResponse& pos)
{
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

std::vector<uint8_t> Server::encodeGameStartedResponse(const GameStartedResponse& pos)
{
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

std::vector<uint8_t> Server::encodeGamePausedResponse(const GamePausedResponse& pos)
{
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

std::vector<uint8_t> Server::encodeGameStateResponse(const GameStateResponse& pos)
{
    std::vector<uint8_t> buffer;

    buffer.emplace_back(0x35);

    /// 
    buffer.emplace_back(pos.num_disconnected & 0xFF);
    for (const auto& id: pos.ids) {
        buffer.emplace_back((id >> 24) & 0xFF);
        buffer.emplace_back((id >> 16) & 0xFF);
        buffer.emplace_back((id >> 8) & 0xFF);
        buffer.emplace_back(id & 0xFF);
    }
    return buffer;
}

std::vector<uint8_t> Server::encodeEnemySpawnedResponse(const EnemySpawnedResponse& pos)
{
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

std::vector<uint8_t> Server::encodedEnemyFiredResponse(const EnemyFiredResponse& pos)
{
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

std::vector<uint8_t> Server::encodeEnemyDiedResponse(const EnemyDiedResponse& pos)
{
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

std::vector<uint8_t> Server::encodeCollisionResponse(const CollisionResponse& pos)
{
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
