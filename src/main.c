#include "gt/gametank.h"
#include "gt/input.h"
#include "gt/gfx/draw_queue.h"
#include "gt/audio/music.h"

#include "./player.h"
#include "./map.h"
#include "./zombies.h"

char box_x = 30, box_y = 20;
char dx = 1, dy = 1;

int main () {
 
    init_audio_coprocessor();
    init_music();

    init_graphics();
    init_player();
    init_zombies();
    load_tile_graphics();
 
    while (1) {                                     //  Run forever
        
        draw_tile_map();
        draw_player();
        draw_zombies();
        queue_clear_border(0);
        
        update_player();
        update_zombies();
 
        await_draw_queue();
        await_vsync(1);
        flip_pages();
        update_inputs();
        tick_music();
    }
 
  return (0);                                     //  We should never get here!
}