/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "Network.hpp"

class Server {
  private:
    int p_;
    MoveResponse response;
    MoveRequest move;
    NetworkManager server_;

  public:
    Server(int p);
    ~Server();
    MoveResponse getMove();
    MoveResponse recupMove(
        const std::pair<std::vector<uint8_t>, asio::ip::udp::endpoint>& a,
        MoveRequest& m
    );

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
    std::vector<uint8_t> encodeEnemyMovedResponse(const EnemyMovedResponse& pos
    );
    std::vector<uint8_t> encodedEnemyFiredResponse(const EnemyFiredResponse& pos
    );
    std::vector<uint8_t> encodeEnemyDiedResponse(const EnemyDiedResponse& pos);
    std::vector<uint8_t> encodeCollisionResponse(const CollisionResponse& pos);
};

#endif /* !SERVER_HPP_ */
