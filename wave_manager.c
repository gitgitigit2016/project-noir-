#include "wave_manager.h"

#include "globals.h"
#include "spawn.h"
#include "constants.h"
#include "utils.h"

#include "raylib.h"
#include <stdio.h>

void update_wave_timer(float dt)
{
    
       // Start a new wave when:
       // - no wave is active , we still have waves remaing ,countdown reaches zero
        
        
    


    if(G.wave_active)
        return;


    if(G.wave >= WAVE_COUNT)
        return;



    G.wave_timer -= dt;



    if(G.wave_timer <= 0.f)
    {
        G.wave++;


        spawn_wave(
            G.wave
        );



        char buf[64];


        snprintf(
            buf,
            63,
            "Wave %d incoming!",
            G.wave
        );


        set_message(
            buf,
            2.f
        );
    }
}





void check_wave_completion(void)
{
    
      //  Wave is finished only when:

      //  1. All enemies for this wave have spawned
       // 2. No active enemies remain
    



    if(!G.wave_active)
        return;


    if(G.wave <= 0)
        return;


    if(G.enemies_this_wave <= 0)
        return;


    if(G.enemies_spawned < G.enemies_this_wave)
        return;




    bool enemy_alive = false;



    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(G.enemies[i].active)
        {
            enemy_alive = true;
            break;
        }
    }



    if(enemy_alive)
        return;




    /*
        Everything is dead.
        Wave cleared.
    */


    G.wave_active = false;


    G.gold += 5;




    if(G.wave >= WAVE_COUNT)
    {
        G.phase = PHASE_WIN;
    }
    else
    {
        G.wave_timer = 5.f;



        char buf[64];



        snprintf(
            buf,
            63,
            "Wave %d cleared! +5 gold",
            G.wave
        );



        set_message(
            buf,
            2.5f
        );
    }
}