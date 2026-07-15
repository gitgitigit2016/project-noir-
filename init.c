#include "init.h"
void init_game(void) {
    srand((unsigned)time(NULL));
    memset(&G, 0, sizeof(G));
    G.phase    = PHASE_MAP;
    G.gold     = 15;
    G.safe_hp  = 3;
    G.selected = UNIT_GUARD;
}

void init_kitchen(void) {
    float total_w = SCREEN_W * 0.75f;
    float total_h = SCREEN_H * 0.52f;
    G.cell_w  = total_w / GRID_COLS;
    G.cell_h  = total_h / GRID_ROWS;
    G.grid_ox = (SCREEN_W - total_w) / 2.f;
    G.grid_oy = 110.f;

    memset(G.units,     0, sizeof(G.units));
    memset(G.enemies,   0, sizeof(G.enemies));
    G.unit_count        = 0;
    G.enemy_count       = 0;
    G.wave              = 0;
    G.wave_timer        = 4.f;
    G.wave_active       = false;
    G.safe_hp           = 3;
    G.gold              = 15;
    G.enemies_spawned   = 0;
    G.enemies_this_wave = 0;
    G.clue_tile_count = 3;
    G.clue_tiles[0] = (ClueTile){ 3, 0 };
    G.clue_tiles[1] = (ClueTile){ 6, 1 };
    G.clue_tiles[2] = (ClueTile){ 9, 2 };

    set_message("Defend the Kitchen! Place Guards and Cooks.", 3.f);
}
