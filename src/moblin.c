#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>

#include "moblin.h"
#include "graphics/Moblin.h"
#include "common.h"

#define MOBLIN_SPEED 4
#define LINK_PADDED_TILE_COUNT 12
#define MOBLIN_COUNTER_RESET 255u
#define MOBLIN_COUNTER_WALK_LIMIT 128u

uint8_t moblinDirection = 0;
uint16_t moblinX, moblinY;
uint16_t moblinCounter = MOBLIN_COUNTER_RESET;

// Keep track of which metasprite to use for moblin
metasprite_t const *moblinMetasprite;

void SetupMoblin(){
    // set down tiles in VRAM
    // we start adding after LINK_PADDED_TILE_COUNT
    set_sprite_data(LINK_PADDED_TILE_COUNT, Moblin_TILE_COUNT, Moblin_tiles);

    // position near the bottom left
    // scale position since using scaled integers
    moblinX = 80 << 4;
    moblinY = 100 << 4;

    // start by facing down
    moblinDirection = 0;

    // start with down metasprite
    moblinMetasprite = Moblin_metasprites[0];
}

uint8_t UpdateMoblin(uint8_t lastSprite){
    // check if moblinCounter is near zero
    if(moblinCounter <= 1){
        // Reset moblin counter
        moblinCounter = MOBLIN_COUNTER_RESET;

        // Pick new random direction
        // use DIV register for pseudo random value
        // use value correspond with 'twoFrameDirections' array
        // each value represent down, up, right, left
        const uint8_t directionsOnly[] = {0, 2, 4, 6};
        moblinDirection = directionsOnly[DIV_REG % 4];
    }else{
        // Decrease counter
        moblinCounter--;
    }

    // moblin is moving when the counter above 256
    if(moblinCounter > MOBLIN_COUNTER_WALK_LIMIT){
        // change x and y position based on moving direction
        moblinX += directionsForTwoFrameObjects[moblinDirection].x * MOBLIN_SPEED;
        moblinY += directionsForTwoFrameObjects[moblinDirection].y * MOBLIN_SPEED;

        // set proper metasprite for moblin nased on its moving direction
        // each direction has 2 metasprite
        moblinMetasprite = Moblin_metasprites[moblinDirection + twoFrameRealValue];
    }

    return move_metasprite(moblinMetasprite, LINK_PADDED_TILE_COUNT, lastSprite, moblinX >> 4, moblinY >> 4);
}
