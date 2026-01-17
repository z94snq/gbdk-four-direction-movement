#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>

#include "cat.h"
#include "graphics/Cat.h"
#include "common.h"

#define CAT_SPEED 4
#define CAT_PADDED_TILE_COUNT 12
#define CAT_COUNTER_RESET 255u
#define CAT_COUNTER_WALK_LIMIT 128u

uint8_t catDirection = 0;
uint16_t catX, catY;
uint16_t catCounter = CAT_COUNTER_RESET;
uint8_t flipCat = FALSE;

// Keep track of which metasprite to use for cat
metasprite_t const *catMetasprite;

void SetupCat(){
    // set down tiles in VRAM
    // we start adding after CAT_PADDED_TILE_COUNT
    set_sprite_data(CAT_PADDED_TILE_COUNT, Cat_TILE_COUNT, Cat_tiles);

    // position near the bottom left
    // scale position since using scaled integers
    catX = 80 << 4;
    catY = 100 << 4;

    // start by facing down
    catDirection = 0;

    // start with down metasprite
    catMetasprite = Cat_metasprites[0];
}

uint8_t UpdateCat(uint8_t lastSprite){
    // check if catCounter is near zero
    if(catCounter <= 1){
        // Reset cat counter
        catCounter = CAT_COUNTER_RESET;

        // Pick new random direction
        // use DIV register for pseudo random value
        // use values corresponding to 'directionsForThreeFrameObjects' array
        // each value represents down, up, right, left (indices for 3 frames per direction)
        const uint8_t directionsOnly[] = {0, 3, 6, 9};
        catDirection = directionsOnly[DIV_REG % 4];
    }else{
        // Decrease counter
        catCounter--;
    }

    // cat is moving when the counter above 256
    if(catCounter > CAT_COUNTER_WALK_LIMIT){
        // change x and y position based on moving direction
        catX += directionsForThreeFrameObjects[catDirection].x * CAT_SPEED;
        catY += directionsForThreeFrameObjects[catDirection].y * CAT_SPEED;

        // set proper metasprite for cat based on its moving direction
        // left direction uses flipped right sprites
        if(catDirection == 9){
            // Left: use right sprites (index 6) and flip
            catMetasprite = Cat_metasprites[6 + threeFrameRealValue];
            flipCat = TRUE;
        }else{
            // Down (0), Up (3), Right (6)
            catMetasprite = Cat_metasprites[catDirection + threeFrameRealValue];
            flipCat = FALSE;
        }
    }

    // flip metasprite along vertical axis if needed
    if(flipCat){
        return move_metasprite_vflip(catMetasprite, CAT_PADDED_TILE_COUNT, lastSprite, catX >> 4, catY >> 4);
    }else{
        return move_metasprite(catMetasprite, CAT_PADDED_TILE_COUNT, lastSprite, catX >> 4, catY >> 4);
    }
}
