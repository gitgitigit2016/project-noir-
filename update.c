#include "update.h"
void update_defense(void) {
    float dt = GetFrameTime();
    G.game_time     += dt;
    G.message_timer -= dt;
    if (!G.wave_active && G.wave < WAVE_COUNT) {
        G.wave_timer -= dt;
        if (G.wave_timer <= 0.f) {
            G.wave++;
            spawn_wave(G.wave);
            char buf[64];
            snprintf(buf, 63, "Wave %d incoming!", G.wave);
            set_message(buf, 2.f);
        }
    }
    if (G.wave_active && G.enemies_spawned < G.enemies_this_wave) {
        G.spawn_timer -= dt;
        if (G.spawn_timer <= 0.f) {
            G.spawn_timer = 1.3f;
            spawn_enemy(randi(0, GRID_ROWS - 1));
            G.enemies_spawned++;
        }
    }
    float safe_x = G.grid_ox + G.cell_w * 0.5f;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &G.enemies[i];
        if (!e->active) continue;
        float target_y = G.grid_oy + e->lane * G.cell_h + G.cell_h * 0.5f;
        e->y += (target_y - e->y) * 6.f * dt;
        e->x -= e->speed * dt;

        if (e->x < safe_x) {
            e->active = false;
            G.safe_hp--;
            set_message("The safe was breached!", 2.f);
            if (G.safe_hp <= 0) { G.phase = PHASE_GAMEOVER; return; }
        }
        if (e->hp <= 0.f) {
            e->active = false;
            G.gold++;
        }
    }
    for (int i = 0; i < G.unit_count; i++) {
        Unit *u = &G.units[i];
        if (!u->active) continue;
        u->attack_timer -= dt;
        if (u->attack_timer > 0.f) continue;
        float range = (u->type == UNIT_GUARD) ? G.cell_w * 3.5f : G.cell_w * 1.8f;
        int   dmg   = (u->type == UNIT_GUARD) ? 2 : 3;
        float aspd  = (u->type == UNIT_GUARD) ? 1.1f : 0.75f;

        float best = 1e9f; int bi = -1;
        for (int ei = 0; ei < MAX_ENEMIES; ei++) {
            Enemy *e = &G.enemies[ei];
            if (!e->active) continue;
            if (u->type == UNIT_GUARD && e->lane != u->row) continue;
            float dx = e->x - u->x, dy = e->y - u->y;
            float d  = sqrtf(dx*dx + dy*dy);
            if (d < range && d < best) { best = d; bi = ei; }
        }
        if (bi >= 0) {
            G.enemies[bi].hp -= (float)dmg;
            u->attack_timer = aspd;
            if (u->type == UNIT_COOK) {
                for (int ei = 0; ei < MAX_ENEMIES; ei++) {
                    if (ei == bi || !G.enemies[ei].active) continue;
                    float dx = G.enemies[ei].x - u->x, dy = G.enemies[ei].y - u->y;
                    if (sqrtf(dx*dx+dy*dy) < range) G.enemies[ei].hp -= dmg * 0.5f;
                }
            }
            Color pc = (u->type==UNIT_GUARD)?(Color){255,255,100,255}:(Color){255,140,50,255};
        }
    }
    if (G.wave_active && G.wave > 0 && G.enemies_this_wave > 0
        && G.enemies_spawned >= G.enemies_this_wave) {
        bool any = false;
        for (int i = 0; i < MAX_ENEMIES; i++)
            if (G.enemies[i].active) { any = true; break; }
        if (!any) {
            G.wave_active = false;
            G.gold += 5;
            if (G.wave >= WAVE_COUNT) {
                G.phase = PHASE_WIN;
            } else {
                G.wave_timer = 5.f;
                char buf[64];
                snprintf(buf, 63, "Wave %d cleared! +5 gold", G.wave);
                set_message(buf, 2.5f);
            }
        }
    }
    Vector2 mp = GetMousePosition();
    float shop_y = SCREEN_H - 95.f;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (mp.x>20&&mp.x<140&&mp.y>shop_y&&mp.y<shop_y+75) {
            G.selected = UNIT_GUARD; G.place_mode = true;
        } else if (mp.x>150&&mp.x<270&&mp.y>shop_y&&mp.y<shop_y+75) {
            G.selected = UNIT_COOK; G.place_mode = true;
        } else {
            int col, row;
            if (G.place_mode && pixel_to_grid(mp, &col, &row))
                place_unit(G.selected, col, row);
        }
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) G.place_mode = false;
}
