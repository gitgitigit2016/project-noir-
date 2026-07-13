#include "renderer.h"

#include "game_state.h"
#include "characters.h"

#include "raylib.h"
#include <math.h>



void draw_grid(void)
{
    for(int r = 0; r < GRID_ROWS; r++)
    {
        for(int c = 0; c < GRID_COLS; c++)
        {
            Rectangle rect =
            {
                G.grid_ox + c * G.cell_w,
                G.grid_oy + r * G.cell_h,
                G.cell_w,
                G.cell_h
            };


            DrawRectangleLinesEx(
                rect,
                1,
                GRAY
            );
        }
    }
}




void draw_clue_tiles(void)
{
    for(int i = 0; i < G.clue_tile_count; i++)
    {
        Vector2 p = grid_to_pixel(
            G.clue_tiles[i].col,
            G.clue_tiles[i].row
        );


        DrawCircle(
            p.x,
            p.y,
            12,
            YELLOW
        );
    }
}




void draw_units_grid(void)
{
    float scale =
        fminf(
            G.cell_w,
            G.cell_h
        ) * 0.17f;


    if(scale < 4)
        scale = 4;



    for(int i = 0; i < G.unit_count; i++)
    {

        if(!G.units[i].active)
            continue;



        if(G.units[i].type == UNIT_GUARD)
        {
            draw_guard(
                G.units[i].x,
                G.units[i].y,
                scale
            );
        }
        else
        {
            draw_cook(
                G.units[i].x,
                G.units[i].y,
                scale
            );
        }
    }
}





void draw_enemies_grid(void)
{

    float scale = 5;



    for(int i = 0; i < G.enemy_count; i++)
    {

        if(!G.enemies[i].active)
            continue;



        float bob =
            sinf(
                G.enemies[i].anim_timer
            ) * 2;



        if(G.enemies[i].type == ENEMY_SUSPECT)
        {
            draw_suspect(
                G.enemies[i].x,
                G.enemies[i].y,
                scale,
                bob
            );
        }
        else
        {
            draw_brute(
                G.enemies[i].x,
                G.enemies[i].y,
                scale,
                bob
            );
        }
    }
}





void draw_particles_all(void)
{

    for(int i = 0; i < MAX_PARTICLES; i++)
    {

        if(G.particles[i].life <= 0)
            continue;



        float alpha =
            G.particles[i].life /
            G.particles[i].max_life;



        DrawCircle(
            G.particles[i].x,
            G.particles[i].y,
            G.particles[i].size,
            Fade(
                G.particles[i].color,
                alpha
            )
        );
    }
}