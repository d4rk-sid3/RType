/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#ifndef NETWORK_HPP_
#define NETWORK_HPP_
#include <asio.hpp>
#include <cstdlib>
#include <ctype.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <limits.h>
#include <poll.h>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <SFML/Graphics.hpp>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>

enum Direction : uint8_t { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

enum BULLET_TYPE : uint8_t { SOLIDE = 0, NONE = 1 };

struct Vector2D {
    uint16_t x;
    uint16_t y;
};

enum EnemyType : uint8_t { TYPE_1 = 1, TYPE_2 = 2, TYPE_3 = 3, TYPE_4 = 4 };

struct Timestamp {
    uint64_t milliseconds;
};

// Serveur -> Client
struct EnemyMovedResponse {
    uint8_t type; // 0x37
    uint16_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
};

// Serveur -> Client
struct NbrEntity {
    uint8_t type; // 0x38
    uint8_t nbr;
};


// // Client -> Serveur
// struct MoveRequest {
//     uint8_t type; // 0x23
//     Direction direction;
// };

// // Serveur -> Client
// struct MoveResponse {
//     uint8_t type; // 0x24
//     uint32_t player_id;
//     Vector2D direction;
//     Vector2D position;
//     u_int8_t speed;
//     Timestamp timestamp;
// };

// // Client -> Serveur
// struct ShootRequest {
//     uint8_t type; // 0x25
// };

// // Serveur -> Client
// struct ShootResponse {
//     uint8_t type; // 0x26
//     u_int32_t player_id;
//     Vector2D bullet_position;
//     Vector2D bullet_direction;
//     u_int8_t bullet_speed;
//     BULLET_TYPE bullet;
// };

// // Client -> Serveur
// struct PickupItemResquest {
//     uint8_t type; // ??
// };

// // Serveur -> Client
// struct PickupItemResponse {
//     uint8_t type; // 0x27
//     uint32_t player_id;
//     uint32_t item_id;
//     Vector2D item_position;
//     Timestamp timestamp;
// };

// enum State : uint8_t { DEATH = 0, ALIVE = 1 };

// enum GameState : uint8_t { PAUSE = 0, IN_GAME = 1 };

// enum BossState : uint8_t { LOST = 0, WON = 1 };

// enum PauseState : uint8_t { PAUSED = 0, RESUMED = 1 };

// enum CollisionType : uint8_t {
//     BULLET_BULLET = 0,
//     BULLET_OBSTACLE = 1,
//     BULLET_PLAYER = 2,
//     BULLET_ENEMY = 3,
//     PLAYER_ENEMY = 4
// };

// // Serveur -> Client
// struct PlayerStateResponse {
//     uint8_t type; // 0x28
//     uint32_t player_id;
//     uint16_t remaining_health;
//     uint32_t score;
//     uint8_t current_level;
//     State state;
// };

// // Serveur -> Client
// struct PlayerGameStateResponse {
//     uint8_t type; // 0x29
//     uint32_t player_id;
//     uint16_t remaining_health;
//     uint32_t score;
//     uint8_t current_level;
//     State state;
//     GameState game_state;
// };

// // Serveur -> Client
// struct BeatBossResponse {
//     uint8_t type; // 0x30
//     uint32_t player_id;
//     uint32_t boss_id;
//     Vector2D player_position;
//     Timestamp timestamp;
//     BossState boss_state;
// };

// // Server -> Client
// struct CheckpointResponse {
//     uint8_t type; // 0x31
//     uint32_t player_id;
//     uint32_t checkpoint_id;
//     Vector2D player_position;
//     Timestamp timestamp;
// };

// // Serveur -> Client
// struct GameStartedResponse {
//     uint8_t type; // 0x32
//     uint32_t player_id;
//     uint32_t checkpoint_id;
//     Vector2D player_position;
//     Timestamp timestamp;
// };

// // Client → Serveur
// struct GamePausedRequest {
//     uint8_t type; // 0x33
// };

// // Serveur -> Client
// struct GamePausedResponse {
//     uint8_t type; // 0x34
//     Timestamp timestamp;
//     PauseState current_state;
// };

// // Serveur -> Client
// struct GameStateResponse {
//     uint8_t type; // 0x35
//     uint8_t num_disconnected;
//     std::vector<uint8_t> ids;
// };

// // Serveur -> Client
// struct EnemySpawnedResponse {
//     uint8_t type; // 0x36
//     uint32_t enemy_id;
//     EnemyType enemy_type;
//     Vector2D position;
//     Vector2D direction;
//     Timestamp timestamp;
// };

// struct EnemyFiredResponse {
//     uint8_t type; // 0x38
//     uint32_t enemy_id;
//     EnemyType enemy_type;
//     Vector2D position;
//     Vector2D direction;
//     Timestamp timestamp;
// };

// struct EnemyDiedResponse {
//     uint8_t type; // 0x39
//     uint32_t enemy_id;
//     EnemyType enemy_type;
//     Vector2D position;
//     Timestamp timestamp;
// };

// struct CollisionResponse {
//     uint8_t type;         // 0x40
//     uint32_t entity_id_1; // Premier ID d'entité
//     uint32_t entity_id_2; // Deuxième ID d'entité
//     Vector2D position;    // Position du choc
//     CollisionType collision_type;
//     Timestamp timestamp;
// };

class NetworkManager {
  public:
    NetworkManager(int port, std::string address);

    NetworkManager(int port);

    ~NetworkManager();

    void poll();
    void run();
    void receive();
    void send(
        const std::vector<u_int8_t>& msg, size_t size,
        const asio::ip::udp::endpoint& client
    );
    std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint> getLastMsg();
    asio::ip::udp::endpoint getLastSender() const;
    asio::io_context& getContext() {
        return context;
    };

  protected:
  private:
    asio::io_context context;
    asio::ip::udp::socket socket;
    std::array<u_int8_t, 1024> buff{};
    asio::ip::udp::endpoint last_sender_;
    asio::ip::udp::endpoint server_endpoint_;
    bool isrunning;
    std::vector<u_int8_t> lastmsg;
    std::queue<std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint>>
        messages;
};

#endif /* !NETWORK_HPP_ */
