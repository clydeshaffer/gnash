#include "gt/gametank.h"
#include "gt/gfx/sprites.h"
#include "gt/gfx/draw_direct.h"
#include "gt/banking.h"
#include "gen/assets/gfx.h"

/* Here is a tilemap rendering file that serves as an example of the Direct Drawing API
After calling direct_prepare_sprite_mode or direct_prepare_box_mode the blitter registers
are written to using vram[{registerName}]. When the parametrs are ready, use DIRECT_DRAW_START()
to start the blit which will also set the draw_busy flag so that await_drawing() functions correctly. */

SpriteSlot map_tile_gfx;

void load_tile_graphics() {
    map_tile_gfx = allocate_sprite(&ASSET__gfx__tileset_blue_bmp_load_list);

}

void draw_tile_map() {
    static char x, y, i, t;
    direct_prepare_sprite_mode(map_tile_gfx);
    direct_transparent_mode(false);
    push_rom_bank();
    change_rom_bank(ASSET__gfx__test1_bin_bank);

    x = 0;
    y = 0;
    i = 0;
    DIRECT_SET_DEST_Y(0);
    DIRECT_SET_WIDTH(16);
    DIRECT_SET_HEIGHT(16);
    while(i < 64) {
        t = ASSET__gfx__test1_bin_ptr[i];
        await_drawing();
        DIRECT_SET_DEST_X(x);
        DIRECT_SET_SOURCE_X(t << 4);
        DIRECT_SET_SOURCE_Y(t & ~15);
        DIRECT_DRAW_START();
        ++i;
        x += 16;
        if(x == 128) {
            x = 0;
            y += 16;
            DIRECT_SET_DEST_Y(y); //Incidentally, VY, GY, and Height are safe to set while a blit is still running.
        }
    }
    await_drawing();

    pop_rom_bank();
}