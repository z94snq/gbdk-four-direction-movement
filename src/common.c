#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>
#include "common.h"

// Easy way to determine a direction for an object with three frames per direction
const Vector8 directionsForThreeFrameObjects[12]={
    {0, 1}, // Down frame 1
    {0, 0}, // Down frame 2
    {0, 0}, // Down frame 3
    {0, -1}, // Up frame 1
    {0, 0}, // Up frame 2
    {0, 0}, // Up frame 3
    {1, 0}, // Right frame 1
    {0, 0}, // Right frame 2
    {0, 0}, // Right frame 3
    {-1, 0}, // Left frame 1
    {0, 0}, // Left frame 2
    {0, 0}, // Left frame 3
};

uint8_t joypadCurrent = 0;
uint8_t threeFrameCounter = 0;
uint8_t threeFrameRealValue = 0;

/**
 * @brief Both astronaut and cat use three frame walk animation. They share this variable that determines which frame to show when walking.
 */
void UpdateThreeFrameCounter(){
    threeFrameCounter += 2;
    threeFrameRealValue = threeFrameCounter>>4;

    // Stop & reset if the value is over 3
    if(threeFrameRealValue >= 3){
        threeFrameRealValue = 0;
        threeFrameCounter = 0;
    }
}
