/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_
#include <algorithm>
#include <vector>

#include "../../utility_classes/include/Network.hpp"
#include "Factory.hpp"
#include "registry.hpp"
#include "Factory.hpp"
#include <map>

#define WINDOW_WIDTH 738
#define WINDOW_HEIGHT 432

inline int player_entity_id = 0;

typedef struct entity_info_s {
    entity entity_id;
    std::string type;
    double spawn_time;
    double spawn_y;
} entity_info_t;

typedef enum { MENU, TRANSITION, GAME, GAME_OVER } state_t;

typedef struct menu_info_s {
    entity background;
    entity title;
    entity start_text;
    entity menu_background_music;
    entity menu_fade_in_rect;
    entity menu_fade_out_rect;
} menu_info_t;

class Client {
  private:
    registry& _reg;
    int port_;
    menu_info_t menu_info;
    double levelTimer = 0.0;
    // MoveRequest move;
    // MoveResponse check;
    NetworkManager client_;

    // This map associates the servers_ids to the client_ids in the registry
    std::unordered_map<size_t, size_t> ids_assoc;

    // std::vector<...> old;
    // std::vector<...> new;

    public:
    state_t state = GAME;
    Client(int p, std::string a, registry &reg);
    ~Client();

    void initMenu();
    void runMenu(double delta);
    void initGame();
    void runLevel(double delta);
    void sendPlayerInput();
    
    // decodeur
    NbrEntity decodeNbrEntity(const std::vector<uint8_t>& buffer);
    EnemyMovedResponse decodeEnemyMovedResponse(const std::vector<uint8_t>& buffer);


    /*MoveResponse decodeMoveResponse(const std::vector<uint8_t>& buffer);
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
    EnemyFiredResponse 
    decodeEnemyFiredResponse(const std::vector<uint8_t>& buffer);
    EnemyDiedResponse decodeEnemyDiedResponse(const std::vector<uint8_t>& buffer
    );
    CollisionResponse decodeCollisionResponse(const std::vector<uint8_t>& buffer
    );*/

    // encodeur
    /*std::vector<uint8_t> encodeMoveResquest(const MoveRequest& pos);
    std::vector<uint8_t> encodeShootResquest(const ShootRequest& pos);
    std::vector<uint8_t> encodePickupItemResquest(const PickupItemResquest& pos
    );
    std::vector<uint8_t> encodeGamePausedRequest(const GamePausedRequest& pos);*/
};

#endif /* !CLIENT_HPP_ */
