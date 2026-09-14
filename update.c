#include "update.h"
#include "init.h"
#include "spawn.h"
#include "utils.h"

static void open_info(const char *title, const char *text) {
    G.popup_title = title;
    G.popup_text = text;
    G.panel = PANEL_INFO;
    G.placing_unit = false;
}

static void maybe_unlock_rumor(void) {
    int expected = G.kills / KILL_RUMOR_STEP;
    // expected which rumour are we in

    if (expected > G.rumor_count && G.rumor_count < TOTAL_RUMORS) {
        int id = G.rumor_count;
        G.rumor_count++;
        open_info("RUMOR - UNVERIFIED", RUMORS[id]);
    }
}

void collect_evidence(int id) {
    if (id < 0 || id >= TOTAL_EVIDENCE) return;

    Evidence *e = &EVIDENCE[id];

    if (e->collected) {
        set_message("That location has already been searched.", 2.0f);
        return;
    }

    if (!evidence_unlocked(id)) {
        set_message("That evidence location is not available yet.", 2.0f);
        return;
    }

    if (G.credits < e->cost) {
        set_message("Not enough Credits to search that location.", 2.0f);
        return;
    }

    G.credits -= e->cost;

    e->collected = true;
    G.evidence_count++;

    if (e->major) {
        open_info("MAJOR EVIDENCE DISCOVERED", e->text);
    } else {
        open_info(e->title, e->text);
    }
}

bool place_unit(UnitType type, int col, int row) {
    int cost = unit_cost(type);

    if (G.credits < cost) {
        set_message("Not enough Credits.", 1.5f);
        return false;
    }

    if (col <= 0) {
        set_message("The Archive column is protected space.", 1.5f);
        return false;
    }

    if (col >= GRID_COLS - 1) {
        set_message("Enemy entry column cannot hold defenders.", 1.5f);
        return false;
    }

    if (cell_has_unit(col, row)) {
        set_message("That cell already has a defender.", 1.5f);
        return false;
    }

    int evidence_id = evidence_at_cell(col, row);

    if (evidence_id >= 0 && !EVIDENCE[evidence_id].collected) {
        set_message("That cell contains a clue location.", 1.5f);
        return false;
    }

    if (G.unit_count >= MAX_UNITS) {
        set_message("Defender limit reached.", 1.5f);
        return false;
    }

    Vector2 p = grid_center(col, row);

    Unit *u = &G.units[G.unit_count++];

    u->type = type;
    u->col = col;
    u->row = row;

    u->x = p.x;
    u->y = p.y;

    u->attack_timer = 0.0f;
    u->active = true;

    // Bullet starts inactive
    u->shot_active = false;
    u->shot_progress = 0.0f;

    G.credits -= cost;

    return true;
}

void update_units(float dt) {
    for (int i = 0; i < G.unit_count; i++) {
        Unit *u = &G.units[i];

        if (!u->active) continue;

        // UPDATE  BULLET

        if (u->shot_active) {
            u->shot_progress += dt * 2.0f; // 0 to 1 how much % area has it covered

            if (u->shot_progress >= 1.0f) {
                u->shot_progress = 1.0f;
                u->shot_active = false;
            }
        }

        // NORMAL ATTACK TIMER

        u->attack_timer -= dt; // this tells us how much to wait until the attack hits

        if (u->attack_timer > 0.0f)
            continue;

        float range;
        float cooldown;
        float damage;

        if (u->type == UNIT_GUARD) {
            range = G.cell_w * 4.0f;
            cooldown = 0.85f;
            damage = 2.0f;
        } else {
            range = G.cell_w * 1.7f;
            cooldown = 0.65f;
            damage = 3.0f;
        }

        int best_id = -1;
        float best_distance = 999999.0f;

        for (int j = 0; j < MAX_ENEMIES; j++) {
            Enemy *e = &G.enemies[j];

            if (!e->active) continue;

            if (u->type == UNIT_GUARD && e->lane != u->row)
                continue;

            float dx = e->x - u->x;
            float dy = e->y - u->y;

            float d = sqrtf(dx * dx + dy * dy);

            if (d < range && d < best_distance) {
                best_distance = d;
                best_id = j;
            }
        }

        if (best_id >= 0) {

            // Existing damage
            G.enemies[best_id].hp -= damage;

            // Guard gets visual bullet
            if (u->type == UNIT_GUARD) {
                u->shot_start_x = u->x;
                u->shot_start_y = u->y;

                u->shot_end_x = G.enemies[best_id].x;
                u->shot_end_y = G.enemies[best_id].y;

                u->shot_progress = 0.0f;
                u->shot_active = true;
            }

            u->attack_timer = cooldown;
        }
    }
}

void update_enemies(float dt) {
    float archive_x = G.grid_x + G.cell_w * 0.55f;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &G.enemies[i];

        if (!e->active) continue;

        if (e->hp <= 0.0f) {
            e->active = false;

            G.kills++;
            G.credits++;

            set_message("Enemy stopped. +1 Credit", 1.0f);

            maybe_unlock_rumor();

            continue;
        }

        e->x -= e->speed * dt;

        if (e->x <= archive_x) {
            e->active = false;

            G.archive_hp--;

            if (G.archive_hp <= 0) {
                G.archive_hp = 0;

                G.state = STATE_LOSE;
                G.wrong_accusation = false;
                G.panel = PANEL_NONE;

                return;
            }

            set_message("The Evidence Archive was hit!", 2.0f);
        }
    }
}

static void handle_grid_click(Vector2 mouse) {
    int col, row;

    if (!pixel_to_grid(mouse, &col, &row))
        return;

    int evidence_id = evidence_at_cell(col, row);

    if (!G.placing_unit && evidence_id >= 0) {
        collect_evidence(evidence_id);
        return;
    }

    if (G.placing_unit && place_unit(G.selected_unit, col, row)) {
        G.placing_unit = false;
    }
}

void update_play_input(void) {
    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        G.placing_unit = false;

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;

    if (point_in_rect(mouse, guard_button_rect())) {
        G.selected_unit = UNIT_GUARD;
        G.placing_unit = true;
        return;
    }

    if (point_in_rect(mouse, cook_button_rect())) {
        G.selected_unit = UNIT_COOK;
        G.placing_unit = true;
        return;
    }

    if (point_in_rect(mouse, case_button_rect())) {
        G.panel = PANEL_CASEFILE;
        G.placing_unit = false;
        return;
    }

    if (point_in_rect(mouse, accuse_button_rect())) {
        if (G.evidence_count >= ACCUSE_REQUIRED) {
            G.panel = PANEL_ACCUSE;
            G.placing_unit = false;
        } else {
            set_message(
                "Collect at least 4/7 Evidence before accusing.",
                2.0f
            );
        }

        return;
    }

    handle_grid_click(mouse);
}

void update_game(void) {
    float dt = GetFrameTime();

    if (G.message_timer > 0.0f)
        G.message_timer -= dt;

    if (G.state == STATE_TITLE) {
        if (
            IsKeyPressed(KEY_ENTER) ||
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        ) {
            start_game();
        }

        return;
    }

    if (G.state == STATE_WIN || G.state == STATE_LOSE) {
        if (IsKeyPressed(KEY_R))
            start_game();

        return;
    }

    if (G.panel != PANEL_NONE)
        return;

    G.game_time += dt;

    update_spawning(dt);
    update_units(dt);
    update_enemies(dt);

    if (G.state == STATE_PLAY)
        update_play_input();
}