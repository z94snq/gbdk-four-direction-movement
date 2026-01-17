#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>

#include "astronaut.h"
#include "graphics/AstronautDown.h"
#include "graphics/AstronautUp.h"
#include "graphics/AstronautRight.h"
#include "graphics/palettes.h"

#define ASTRONAUT_SPEED 5

extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;

extern const int16_t directions[9][2];

uint8_t astronautDirection = 0;
uint16_t astronautX, astronautY;

uint8_t flipAstronaut = FALSE;

// Keep track of which metasprite to use for astronaut
metasprite_t const *astronautMetasprite;

void SetupAstronaut(){
    // set down tiles in VRAM
    set_sprite_data(0, AstronautDown_TILE_COUNT, AstronautDown_tiles);

    // position near the top middle
    // scale position since using scaled integers
    astronautX = 80 << 4;
    astronautY = 30 << 4;

    // start by facing down
    astronautDirection = J_DOWN;

    // start with down metasprite
    astronautMetasprite = AstronautDown_metasprites[0];
}

uint8_t UpdateAstronaut(){
    // save our last direction so we can track directional changes
    uint8_t astronautLastDirection = astronautDirection;
    uint8_t astronautMoving = FALSE;
    astronautDirection = astronautDirection;

    // check if right joypad button is pressed
    if(joypadCurrent & J_RIGHT){
        astronautX += ASTRONAUT_SPEED;
        astronautDirection = J_RIGHT;
        astronautMoving = TRUE;
    }

    // check if left joypad button is pressed
    if(joypadCurrent & J_LEFT){
        astronautX -= ASTRONAUT_SPEED;
        astronautDirection = J_LEFT;
        astronautMoving = TRUE;
    }

    // check if down joypad button is pressed
    if(joypadCurrent & J_DOWN){
        astronautY += ASTRONAUT_SPEED;
        astronautDirection = J_DOWN;
        astronautMoving = TRUE;
    }

    // check if up joypad button is pressed
    if(joypadCurrent & J_UP){
        astronautY -= ASTRONAUT_SPEED;
        astronautDirection = J_UP;
        astronautMoving = TRUE;
    }

    // if astronaut is moving
    if(astronautMoving){
        // if astronaut changed direction
        if(astronautDirection != astronautLastDirection){
            // set sprite data to new side we are facing
            // left will use right tiles and flipped later
            switch (astronautDirection){
            case J_DOWN:
                set_sprite_data(0, AstronautDown_TILE_COUNT, AstronautDown_tiles);
                break;
            
            case J_RIGHT:
                set_sprite_data(0, AstronautRight_TILE_COUNT, AstronautRight_tiles);
                break;

            case J_LEFT:
                set_sprite_data(0, AstronautRight_TILE_COUNT, AstronautRight_tiles);
                break;

            case J_UP:
                set_sprite_data(0, AstronautUp_TILE_COUNT, AstronautUp_tiles);
                break;
            }
        }

        // change astronaut metasprite
        // for 'left' tiles we will flip 'right' tiles
        switch (astronautDirection) {
        case J_DOWN:
            astronautMetasprite = AstronautDown_metasprites[twoFrameRealValue];
            flipAstronaut = FALSE;
            break;
        
        case J_RIGHT:
            astronautMetasprite = AstronautRight_metasprites[twoFrameRealValue];
            flipAstronaut = FALSE;
            break;

        case J_LEFT:
            astronautMetasprite = AstronautRight_metasprites[twoFrameRealValue];
            flipAstronaut = TRUE;
            break;

        case J_UP:
            astronautMetasprite = AstronautDown_metasprites[twoFrameRealValue];
            flipAstronaut = FALSE;
            break;
        }
    }
    
    // flip metasprite along vertical axis
    // return number of sprites used
    if(flipAstronaut){
        return move_metasprite_vflip(astronautMetasprite, 0, 0, astronautX >> 4, astronautY >> 4);
    }else{
        return move_metasprite(astronautMetasprite, 0, 0, astronautX >> 4, astronautY >> 4);
    }
}
