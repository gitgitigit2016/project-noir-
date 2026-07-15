#if 0 

#include "score_popup.h" // .c file exists since linker can create a " double definition " problem 
#include <stdio.h> // Required for snprintf 


static ScorePopup popupPool[MAX_POPUPS];// static means that the function defined in this .c file can only access this ARRAY OF A STRUCT VARIABLE ; ie the elements are struct variables 

// . Initialize the Pool
void InitScorePopups(void) {
    for (int i = 0; i < MAX_POPUPS; i++) {
        popupPool[i].x = 0.0f;
        popupPool[i].y = 0.0f;
        popupPool[i].scoreValue = 0;
        popupPool[i].alpha = 0;
        popupPool[i].isActive = false; // All start turned false to init 
    }
}

//  Spawn a Pop-up
void SpawnScorePopup(float x, float y, int value) {
    // Loop through to find an available slot
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (!popupPool[i].isActive) {
            // Found a free slot - we will now fill the data accordingly which corresponds to the data of the score // when the condition is met while checking in another function 
            popupPool[i].x = x;
            popupPool[i].y = y;
            popupPool[i].scoreValue = value;
            popupPool[i].alpha = 255;      // Start solid color 
            popupPool[i].isActive = true;  
            return;                        // Exit immediately so we dont fill the other ones up only one can be done using the function 
        }
    }
}

// this function is to be done inside the game loop //while (!WindowShouldClose()) // so that the fps ....

void UpdateScorePopups(void) {
    for (int i = 0; i < MAX_POPUPS; i++) {
        // Only update elements that are actively flying on screen
        if (popupPool[i].isActive) {
            popupPool[i].y -= 1.5f;   // Subtract from Y to ascend up // values throuogh guessing // the less the smoother 
            popupPool[i].alpha -= 4;  // Fade out slightly every single frame // these values depend on guessing 

            // Kill Switch- Once it's completely transparent, turn it off
            if (popupPool[i].alpha <= 0) {
                popupPool[i].isActive = false;
            }
        }
    }
}
// this one also in the game loop just after the previuous one 
// . Draw
void DrawScorePopups(void) {
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (popupPool[i].isActive) {
            // Buffer array to format our integer number into viewable text characters
            char textBuffer[16];
            snprintf(textBuffer, sizeof(textBuffer), "+%d", popupPool[i].scoreValue);

            // unsigned char since the function definition says all should be u.char 
            //the alpha is reducing every fps hence the color will be fade
            // 255 215 0 gives us gold 
            Color fadingGold = { 255, 215, 0, (unsigned char)popupPool[i].alpha };

            
            DrawText(textBuffer, (int)popupPool[i].x, (int)popupPool[i].y, 20, fadingGold);
        }
    }
}
    #endif