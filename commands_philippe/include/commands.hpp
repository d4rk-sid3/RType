#ifndef PROTOCOLE_HPP
#define PROTOCOLE_HPP

#include <cstdint>
#include <vector>
#include <iostream>
#include <cstring>

enum State : uint8_t {
    DEATH = 0,
    ALIVE = 1
};

enum GameState : uint8_t {
    PAUSE = 0,
    IN_GAME = 1
};

enum BossState : uint8_t {
    LOST = 0,
    WON = 1
};

enum PauseState : uint8_t {
    PAUSED = 0,
    RESUMED = 1
};

enum EnemyType : uint8_t {
    TYPE_1 = 1,
    TYPE_2 = 2,
    TYPE_3 = 3,
    TYPE_4 = 4
};

enum CollisionType : uint8_t {
    BULLET_BULLET = 0,
    BULLET_OBSTACLE = 1,
    BULLET_PLAYER = 2,
    BULLET_ENEMY = 3,
    PLAYER_ENEMY = 4
};


struct Vector2D {
    uint16_t x;
    uint16_t y;
};


struct Timestamp {
    uint64_t milliseconds;
};

//Serveur -> Client
struct PlayerStateResponse {
    uint8_t type;               // 0x28
    uint32_t player_id;
    uint16_t remaining_health;
    uint32_t score;
    uint8_t current_level;
    State state;
};

//Serveur -> Client
struct PlayerGameStateResponse {
    uint8_t type;               // 0x29
    uint32_t player_id;
    uint16_t remaining_health;
    uint32_t score;
    uint8_t current_level;
    State state;
    GameState game_state;
};

//Serveur -> Client
struct BeatBossResponse {
    uint8_t type;               // 0x30
    uint32_t player_id;
    uint32_t boss_id;
    Vector2D player_position;
    Timestamp timestamp;
    BossState boss_state;
};

//Server -> Client
struct CheckpointResponse {
    uint8_t type;               // 0x31
    uint32_t player_id;
    uint32_t checkpoint_id;
    Vector2D player_position;
    Timestamp timestamp;
};

// Serveur -> Client
struct GameStartedResponse {
    uint8_t type;               // 0x32
    uint32_t player_id;
    uint32_t checkpoint_id;
    Vector2D player_position;
    Timestamp timestamp;
};

// Client → Serveur 
struct GamePausedRequest {
    uint8_t type;               // 0x33
};

struct GamePausedResponse {
    uint8_t type;               // 0x34
    Timestamp timestamp;
    PauseState current_state;
};

struct GameStateResponse {
    uint8_t type;               // 0x35
    uint8_t num_disconnected;
};

struct EnemySpawnedResponse {
    uint8_t type;               // 0x36
    uint32_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
    Vector2D direction;
    Timestamp timestamp;
};

struct EnemyMovedResponse {
    uint8_t type;               // 0x37
    uint32_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
    Vector2D direction;
    Timestamp timestamp;
};

struct EnemyFiredResponse {
    uint8_t type;               // 0x38
    uint32_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
    Vector2D direction;
    Timestamp timestamp;
};

struct EnemyDiedResponse {
    uint8_t type;               // 0x39
    uint32_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
    Timestamp timestamp;
};

struct CollisionResponse {
    uint8_t type;               // 0x40
    uint32_t entity_id_1;       // Premier ID d'entité
    uint32_t entity_id_2;       // Deuxième ID d'entité
    Vector2D position;          // Position du choc
    CollisionType collision_type;
    Timestamp timestamp;
};

#endif