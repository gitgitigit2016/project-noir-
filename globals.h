#ifndef GLOBALS_H
#define GLOBALS_H

/* =========================================================
   globals.h
   ---------
   Declares the single global game state G.
   Every .c file that needs to read or write game data
   just includes this file to get access to G.
   
   The actual G variable is defined once in globals.c.
   Here we just declare it exists (extern = "it lives
   somewhere else, trust me").
   ========================================================= */

#include "types.h"

extern GameState G;

#endif
