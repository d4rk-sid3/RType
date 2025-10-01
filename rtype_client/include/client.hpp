/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include "../../utility_classes/include/Network.hpp"

class Client {
  private:
    int port_;
    MoveRequest move;
    MoveResponse check;
    NetworkManager client_;

  public:
    Client(int p, std::string a);
    ~Client();
    MoveRequest getMoveKey();

    // decodeur
    MoveResponse decodeMoveResponse(const std::vector<uint8_t>& buffer);
    ShootResponse decodeShootResponse(const std::vector<uint8_t>& buffer);
    PickupItemResponse
    decodePickupItemResponse(const std::vector<uint8_t>& buffer);
    PlayerStateResponse
    decodePlayerStateResponse(const std::vector<uint8_t>& buffer);
    PlayerGameStateResponse
    decodePlayerGameStateResponse(const std::vector<uint8_t>& buffer);
    BeatBossResponse decodeBeatBossResponse(const std::vector<uint8_t>& buffer);
    CheckpointResponse
    decodeCheckpointResponse(const std::vector<uint8_t>& buffer);
    GameStartedResponse
    decodeGameStartedResponse(const std::vector<uint8_t>& buffer);
    GamePausedResponse
    decodeGamePausedResponse(const std::vector<uint8_t>& buffer);
    GameStateResponse decodeGameStateResponse(const std::vector<uint8_t>& buffer
    );
    EnemySpawnedResponse
    decodeEnemySpawnedResponse(const std::vector<uint8_t>& buffer);
    EnemyMovedResponse
    decodeEnemyMovedResponse(const std::vector<uint8_t>& buffer);
    EnemyFiredResponse
    decodeEnemyFiredResponse(const std::vector<uint8_t>& buffer);
    EnemyDiedResponse decodeEnemyDiedResponse(const std::vector<uint8_t>& buffer
    );
    CollisionResponse decodeCollisionResponse(const std::vector<uint8_t>& buffer
    );

    // encodeur
    std::vector<uint8_t> encodeMoveResquest(const MoveRequest& pos);
    std::vector<uint8_t> encodeShootResquest(const ShootRequest& pos);
    std::vector<uint8_t> encodePickupItemResquest(const PickupItemResquest& pos
    );
    std::vector<uint8_t> encodeGamePausedRequest(const GamePausedRequest& pos);
};

#endif /* !CLIENT_HPP_ */
