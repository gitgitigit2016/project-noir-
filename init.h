#ifndef INIT_H
#define INIT_H

/* =========================================================
   init.h
   ------
   Declarations for the game initialization functions.
   init_game() resets everything for a fresh start.
   init_kitchen() sets up the kitchen grid and clue tiles.
   ========================================================= */

#include "globals.h"
#include "utils.h"

void init_game(void);
void init_kitchen(void);

#endif
