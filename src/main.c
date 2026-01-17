#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include <stdint.h>

#include "common.h"
#include "graphics/palettes.h"
#include "astronaut.h"
#include "cat.h"

void main(void)
{
    // Turn on sprites
    // Enable 8x16 sprites mode
    SHOW_SPRITES;
    SPRITES_8x16;
    DISPLAY_ON;
    
    // Set color palettes into vram
    set_sprite_palette(0, palettes_PALETTE_COUNT, palettes_palettes);

    // Setup astronaut & cat
    SetupAstronaut();
    SetupCat();

    // Loop forever
    while(1){
        // Get current joypad data
        joypadCurrent = joypad();

        UpdateTwoFrameCounter();

        uint8_t lastSprite = 0;

        lastSprite += UpdateAstronaut();
        lastSprite += UpdateCat(lastSprite);

        // Hide extra sprites
        hide_sprites_range(lastSprite, 40);

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}
