#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"


typedef struct {

    GamePhase phase;

    float cell_w;
    float cell_h;

    float grid_ox;
    float grid_oy;

    Unit units[MAX_UNITS];
    int unit_count;

    Enemy enemies[MAX_ENEMIES];
    int enemy_count;

    int wave;

    float game_time;

    Particle particles[MAX_PARTICLES];

    int gold;
    int safe_hp;

    UnitType selected;
    bool place_mode;

    char message[128];
    float message_timer;

} GameState;


extern GameState G;


#endif