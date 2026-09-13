#include "spawn.h"
#include "utils.h"

void spawn_enemy(void) {
    int slot = -1;// this tells us that nth enemy = slot 
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!G.enemies[i].active) {
            slot = i;
            break;
        }
    }

    if (slot < 0) return;

    float brute_chance = 0.0f; // chances of the enemy to be a brute 
    if (G.evidence_count >= 5) brute_chance = 0.35f;
    else if (G.evidence_count >= 3) brute_chance = 0.20f;

    EnemyType type = (randf01() < brute_chance) ? ENEMY_BRUTE : ENEMY_WALKER;
    // randf01 gives us a random float from 0 to 1 
    int lane = randi(0, GRID_ROWS - 1);
    // to get the lane for the enemy 
    Vector2 p = grid_center(GRID_COLS - 1, lane);

    Enemy *e = &G.enemies[slot];
    e->type = type;
    e->lane = lane;
    e->x = G.grid_x + GRID_COLS * G.cell_w + 20.0f;
    e->y = p.y;
    e->active = true;

    if (type == ENEMY_BRUTE) {
        e->max_hp = 11.0f;
        e->hp = 11.0f;
        e->speed = 33.0f;
    } else {
        e->max_hp = 5.0f;
        e->hp = 5.0f;
        e->speed = 52.0f;
    }
}

float current_spawn_interval(void) {
    // currant spwn intvl = how many seconds lev=ft till next spawn 
    if (G.evidence_count < 3) return 1.75f;
    if (G.evidence_count < 5) return 1.35f;
    return 1.05f;
}

void update_spawning(float dt) {
    G.spawn_timer -= dt;

    if (G.spawn_timer <= 0.0f) {
        spawn_enemy();
        G.spawn_timer = current_spawn_interval();
    }
}
