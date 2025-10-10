/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** Network
*/

#include <ctype.h>
#include <asio.hpp>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

enum Direction : int16_t { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

enum Action : int16_t { SPACE = 0 };

enum BULLET_TYPE : int16_t { SOLIDE = 0, NONE = 1 };

struct Vector2D {
    int16_t x;
    int16_t y;
};

enum EnemyType : int16_t { TYPE_1 = 1, TYPE_2 = 2, TYPE_3 = 3, TYPE_4 = 4, TYPE_5 = 5, TYPE_6 = 6, TYPE_7 = 7, TYPE_8 = 8, TYPE_9 = 9, TYPE_10 = 10 };

extern std::map<std::string, EnemyType> type_map;

// Serveur -> Client
struct EnemyMovedResponse {
    int8_t type; // 0x37
    int16_t enemy_id;
    EnemyType enemy_type;
    Vector2D position;
};

// Serveur -> Client
struct NbrEntity {
    int8_t type; // 0x38
    int16_t nbr;
};

struct MoveResponse {
    int8_t type; // 0x24
    int16_t player_id;
    Direction direction;
};

struct ActionResponse {
    int8_t type; // 0x25
    int16_t player_id;
    Action input;
};
