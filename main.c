#include "init.h"
#include "update.h"
#include "draw.h"

int main(void) {
    init_game();

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "NOIR - Raylib Prototype");

    load_draw_assets();

    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        update_game();

        BeginDrawing();
        draw_game();
        EndDrawing();
    }

    unload_draw_assets();

    CloseWindow();
    return 0;
}