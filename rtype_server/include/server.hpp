/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include <algorithm>
#include <vector>

#include "Factory.hpp"
#include "Network.hpp"
#include "registry.hpp"

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline int player_entity_id = 0;

typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

class Server {
  private:
    int p_;
    registry& reg;
    Factory& factory;
    double levelTimer = 0.0;

    void loadLevel(const std::string& path);
    void spawn_player(void);

    NetworkManager server_;

    std::vector<entity_info_t> entities;
    std::vector<entity> active_entities;

  public:
    void runLevel(double delta);
    Server(int p, registry& reg, Factory& fac);
    ~Server();

    // update
    MoveResponse
    updateMoveResponse(const MoveResponse& oldpos, const MoveResponse& pos);
    ShootResponse
    updateShootResponse(const ShootResponse& oldpos, const ShootResponse& pos);
    PickupItemResponse updatePickupItemResponse(
        const PickupItemResponse& oldpos, const PickupItemResponse& pos
    );

    // decodeur
    MoveRequest decodeMoveRequest(const std::vector<uint8_t>& buffer);
    ShootRequest decodeShootRequest(const std::vector<uint8_t>& buffer);
    PickupItemResquest
    decodePickupItemResquest(const std::vector<uint8_t>& buffer);
    GamePausedRequest decodeGamePausedRequest(const std::vector<uint8_t>& buffer
    );

    // encodeur
    std::vector<uint8_t> Server::encodeNbrEntity(const NbrEntity& pos);
    std::vector<uint8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos
    );



    std::vector<uint8_t> encodeMoveResponse(const MoveResponse& pos);
    std::vector<uint8_t> encodeShootResponse(const ShootResponse& pos);
    std::vector<uint8_t> encodePickupItemResponse(const PickupItemResponse& pos
    );
    std::vector<uint8_t>
    encodePlayerStateResponse(const PlayerStateResponse& pos);
    std::vector<uint8_t>
    encodePlayerGameStateResponse(const PlayerGameStateResponse& pos);
    std::vector<uint8_t> encodeBeatBossResponse(const BeatBossResponse& pos);
    std::vector<uint8_t> encodeCheckpointResponse(const CheckpointResponse& pos
    );
    std::vector<uint8_t>
    encodeGameStartedResponse(const GameStartedResponse& pos);
    std::vector<uint8_t> encodeGamePausedResponse(const GamePausedResponse& pos
    );
    std::vector<uint8_t> encodeGameStateResponse(const GameStateResponse& pos);
    std::vector<uint8_t>
    encodeEnemySpawnedResponse(const EnemySpawnedResponse& pos);
    std::vector<uint8_t> encodedEnemyFiredResponse(const EnemyFiredResponse& pos
    );
    std::vector<uint8_t> encodeEnemyDiedResponse(const EnemyDiedResponse& pos);
    std::vector<uint8_t> encodeCollisionResponse(const CollisionResponse& pos);
};

#endif /* !SERVER_HPP_ */
