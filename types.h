#ifndef TYPES_H
#define TYPES_H
#include "constants.h"

typedef enum {
    STATE_TITLE,
    STATE_PLAY,
    STATE_WIN,
    STATE_LOSE
} GameState;

typedef enum {
    UNIT_GUARD,
    UNIT_COOK
} UnitType;

typedef enum {
    ENEMY_WALKER,
    ENEMY_BRUTE
} EnemyType;

typedef enum {
    PANEL_NONE,
    PANEL_INFO,
    PANEL_CASEFILE,
    PANEL_ACCUSE
} PanelType;

typedef struct {
    UnitType type;
    int col;
    int row;

    float x;
    float y;

    float attack_timer;
    bool active;

    // Visual bullet effect
    bool shot_active;

    float shot_start_x;
    float shot_start_y;

    float shot_end_x;
    float shot_end_y;

    float shot_progress;
    // for bullets 
} Unit;

typedef struct {
    EnemyType type;
    int lane;
    float x;
    float y;
    float hp;
    float max_hp;
    float speed;
    bool active;
} Enemy;

typedef struct {
    const char *title;
    const char *text;
    int col;
    int row;
    int cost;
    bool major;
    bool collected;
} Evidence;

typedef struct {
    const char *name;
    const char *role;
    const char *note;
} Suspect;

typedef struct {
    GameState state;
    PanelType panel;
    float grid_x;
    float grid_y;
    float cell_w;
    float cell_h;
    Unit units[MAX_UNITS];
    Enemy enemies[MAX_ENEMIES];
    int unit_count;
    int credits;
    int archive_hp;
    int kills;
    int evidence_count;
    int rumor_count;
    float spawn_timer;
    float game_time;
    UnitType selected_unit;
    bool placing_unit;
    char message[160];
    float message_timer;
    const char *popup_title;
    const char *popup_text;
    bool wrong_accusation;
} Game;

#endif
