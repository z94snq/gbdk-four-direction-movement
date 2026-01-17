#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>

#include "link.h"
#include "graphics/LinkDown.h"
#include "graphics/LinkUp.h"
#include "graphics/LinkRight.h"
#include "graphics/palettes.h"

#define LINK_SPEED 5

extern uint8_t joypadCurrent, joypadPrevious, twoFrameRealValue;

extern const int16_t directions[9][2];

uint8_t linkDirection = 0;
uint16_t linkX, linkY;

uint8_t flipLink = FALSE;

// Keep track of which metasprite to use for link
metasprite_t const *linkMetasprite;

void SetupLink(){
    // set down tiles in VRAM
    set_sprite_data(0, LinkDown_TILE_COUNT, LinkDown_tiles);

    // position near the top middle
    // scale position since using scaled integers
    linkX = 80 << 4;
    linkY = 30 << 4;

    // start by facing down
    linkDirection = J_DOWN;

    // start with down metasprite
    linkMetasprite = LinkDown_metasprites[0];
}

uint8_t UpdateLink(){
    // save our last direction so we can track directional changes
    uint8_t linkLastDirection = linkDirection;
    uint8_t linkMoving = FALSE;
    linkDirection = linkDirection;

    // check if right joypad button is pressed
    if(joypadCurrent & J_RIGHT){
        linkX += LINK_SPEED;
        linkDirection = J_RIGHT;
        linkMoving = TRUE;
    }

    // check if left joypad button is pressed
    if(joypadCurrent & J_LEFT){
        linkX -= LINK_SPEED;
        linkDirection = J_LEFT;
        linkMoving = TRUE;
    }

    // check if down joypad button is pressed
    if(joypadCurrent & J_DOWN){
        linkY += LINK_SPEED;
        linkDirection = J_DOWN;
        linkMoving = TRUE;
    }

    // check if up joypad button is pressed
    if(joypadCurrent & J_UP){
        linkY -= LINK_SPEED;
        linkDirection = J_UP;
        linkMoving = TRUE;
    }

    // if link is moving
    if(linkMoving){
        // if link changed direction
        if(linkDirection != linkLastDirection){
            // set sprite data to new side we are facing
            // left will use right tiles and flipped later
            switch (linkDirection){
            case J_DOWN:
                set_sprite_data(0, LinkDown_TILE_COUNT, LinkDown_tiles);
                break;
            
            case J_RIGHT:
                set_sprite_data(0, LinkRight_TILE_COUNT, LinkRight_tiles);
                break;

            case J_LEFT:
                set_sprite_data(0, LinkRight_TILE_COUNT, LinkRight_tiles);
                break;

            case J_UP:
                set_sprite_data(0, LinkUp_TILE_COUNT, LinkUp_tiles);
                break;
            }
        }

        // change link metasprite
        // for 'left' tiles we will flip 'right' tiles
        switch (linkDirection) {
        case J_DOWN:
            linkMetasprite = LinkDown_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;
        
        case J_RIGHT:
            linkMetasprite = LinkRight_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;

        case J_LEFT:
            linkMetasprite = LinkRight_metasprites[twoFrameRealValue];
            flipLink = TRUE;
            break;

        case J_UP:
            linkMetasprite = LinkDown_metasprites[twoFrameRealValue];
            flipLink = FALSE;
            break;
        }
    }
    
    // flip metasprite along vertical axis
    // return number of sprites used
    if(flipLink){
        return move_metasprite_vflip(linkMetasprite, 0, 0, linkX >> 4, linkY >> 4);
    }else{
        return move_metasprite(linkMetasprite, 0, 0, linkX >> 4, linkY >> 4);
    }
}
