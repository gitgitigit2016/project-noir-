#if 0 
#ifndef SCORE_POPUP_H // ifnotdefined 
#define SCORE_POPUP_H // this is to avoid REDEFINITION error and for the second time the compiler reads it it skips till endif to save proccessing time 

#include "raylib.h"
#include <stdbool.h>// for bool

// The maximum number of pop-ups that can exist on screen at the exact same time
#define MAX_POPUPS 10


typedef struct {
    float x;             // The horizontal X position
    float y;             // The vertical Y position
    int scoreValue;      // The actual  score to display 
    int alpha;           // Transparency level: 255 (solid) down to 0 (invisible)
    bool isActive;       // true = active , false = dead and also ready to be used 
} ScorePopup;


//  main.c can see and call these functions.

// sets all pop-ups to inactive at game start
void InitScorePopups(void);

// when the particular condition is met , this function takes the inactive popup to popup active 
void SpawnScorePopup(float x, float y, int value);

// Runs the animation physics (moves Y up, decreases Alpha(intensity ))
void UpdateScorePopups(void);

// Iterates through active slots and physically prints the text 
void DrawScorePopups(void);
#endif
#endif 
