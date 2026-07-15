#include "spawn.h"
bool place_unit(UnitType type, int col, int row) {
    int cost = (type == UNIT_GUARD) ? COST_GUARD : COST_COOK;
    if (G.gold < cost)         { set_message("Not enough gold!", 1.5f);           return false; }
    if (col <= 0)              { set_message("Can't place on Safe column!", 1.5f); return false; }
    if (col >= GRID_COLS - 1)  { set_message("Can't place on entry column!", 1.5f); return false; }
    if (cell_has_unit(col,row)){ set_message("Cell already occupied!", 1.5f);     return false; }
    if (G.unit_count >= MAX_UNITS) return false;
    Vector2 p = grid_to_pixel(col, row);
    G.units[G.unit_count++] = (Unit){ type, col, row, p.x, p.y, 0.f, true };
    G.gold -= cost;
    return true;
}
void spawn_wave(int wave_num) {
    G.wave_active       = true;
    G.enemies_spawned   = 0;
    G.spawn_timer       = 0.f;
    G.enemies_this_wave = 3 + wave_num * 2;
}

void spawn_enemy(int lane) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!G.enemies[i].active) {
            EnemyType t = (randf() > 0.75f && G.wave >= 2) ? ENEMY_BRUTE : ENEMY_SUSPECT;
            float spd   = (t == ENEMY_BRUTE) ? 22.f : 40.f;
            float hp    = (t == ENEMY_BRUTE) ? 12.f :  5.f;
            float sx    = G.grid_ox + GRID_COLS * G.cell_w + 10.f;
            float sy    = G.grid_oy + lane * G.cell_h + G.cell_h * 0.5f;
            G.enemies[i] = (Enemy){ t, sx, sy, lane, spd, hp, hp, true };
            G.enemy_count++;
            return;
        }
    }
}
