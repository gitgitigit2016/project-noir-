#ifndef TYPES_H
#define TYPES_H
#include "constants.h"
typedef enum { PHASE_MAP, PHASE_DEFENSE, PHASE_GAMEOVER, PHASE_WIN } GamePhase;
typedef enum { UNIT_GUARD, UNIT_COOK } UnitType;
typedef enum { ENEMY_SUSPECT, ENEMY_BRUTE } EnemyType;
typedef struct {
    UnitType type;
    int col, row;
    float x, y;
    float attack_timer;
    bool active;
} Unit;
typedef struct {
    EnemyType type;
    float x, y;
    int lane;
    float speed;
    float hp, max_hp;
    bool active;
} Enemy;

typedef struct {
    int col, row;
} ClueTile;
typedef struct {
    GamePhase phase;
    float cell_w, cell_h;
    float grid_ox, grid_oy;
    Unit      units[MAX_UNITS];
    int       unit_count;
    Enemy     enemies[MAX_ENEMIES];
    int       enemy_count;
    int       wave;
    float     wave_timer;
    bool      wave_active;
    int       enemies_this_wave;
    int       enemies_spawned;
    float     spawn_timer;
    ClueTile  clue_tiles[3];
    int       clue_tile_count;
    int       gold;
    int       safe_hp;
    UnitType  selected;
    bool      place_mode;
    char      message[128];
    float     message_timer;
    float     game_time;
} GameState;
#endif
