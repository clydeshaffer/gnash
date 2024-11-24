#include "gt/gfx/sprites.h"
#include "gt/gfx/draw_queue.h"

#include "gen/assets/enemy.h"
#include "gen/assets/enemy/zombie.json.h"

#include "./player.h"

#define MAX_ZOMBIES 4

#define ZOMBIES_SUBFRAME_RATE 8

#define ZOMBIE_FLAG_EXISTS 1

char zombies_x[MAX_ZOMBIES];
char zombies_y[MAX_ZOMBIES];
char zombies_flags[MAX_ZOMBIES];
char zombies_frame[MAX_ZOMBIES];
char zombies_subframe[MAX_ZOMBIES];
char zombies_flip[MAX_ZOMBIES];

char zombies_update_order[MAX_ZOMBIES];

const signed char zombie_target_offset_x[MAX_ZOMBIES] = {0, 16, 255-16, 16};
const signed char zombie_target_offset_y[MAX_ZOMBIES] = {2, 16, 255-16, 255-16};

static char zi, ziu;

SpriteSlot zombie_sprite;

void init_zombies() {
    zombie_sprite = allocate_sprite(&ASSET__enemy__zombie_bmp_load_list);
    set_sprite_frametable(zombie_sprite, ASSET__enemy__zombie_json);
    for(zi = 0; zi < MAX_ZOMBIES; ++zi) {
        zombies_x[zi] = zi << 4;
        zombies_y[zi] = 0;
        zombies_flags[zi] = ZOMBIE_FLAG_EXISTS;
        zombies_frame[zi] = ZOMBIE_TAG_RUN_START;
        zombies_subframe[zi] = zi;
        zombies_flip[zi] = 0;
        zombies_update_order[zi] = zi;
    }
}

void update_zombies() {
    for(zi = 0; zi < MAX_ZOMBIES; ++zi) {
        if(++zombies_subframe[zi] == ZOMBIES_SUBFRAME_RATE) {
            zombies_subframe[zi] = 0;
            if(++zombies_frame[zi] == ZOMBIE_TAG_RUN_END) {
                zombies_frame[zi] = ZOMBIE_TAG_RUN_START;
            }

            if(zombies_flags[zi] & ZOMBIE_FLAG_EXISTS) {
                if(zombies_x[zi] < (player_x + zombie_target_offset_x[zi])) {
                    ++zombies_x[zi];
                    zombies_flip[zi] = SPRITE_FLIP_NONE;
                } else {
                    --zombies_x[zi];
                    zombies_flip[zi] = SPRITE_FLIP_X;
                }
                if(zombies_y[zi] < (player_y + zombie_target_offset_y[zi])) {
                    ++zombies_y[zi];
                } else {
                    --zombies_y[zi];
                }
            }
        }
    }

    for(ziu = 0; ziu < MAX_ZOMBIES-1; ++ziu) {
        if(zombies_y[zombies_update_order[ziu]] > zombies_y[zombies_update_order[ziu+1]]) {
            zi = zombies_update_order[ziu];
            zombies_update_order[ziu] = zombies_update_order[ziu+1];
            zombies_update_order[ziu+1] = zi;
        }
    }
}

void draw_zombies_upper() {
    for(ziu = 0; ziu < MAX_ZOMBIES; ++ziu) {
        zi = zombies_update_order[ziu];
        if(zombies_y[zi] > player_y) {
            return;
        }
        if(zombies_flags[zi] & ZOMBIE_FLAG_EXISTS) {
            queue_draw_sprite_frame(zombie_sprite, zombies_x[zi], zombies_y[zi], zombies_frame[zi], zombies_flip[zi]);
        }
    }
}

void draw_zombies_lower() {
    for(; ziu < MAX_ZOMBIES; ++ziu) {
        zi = zombies_update_order[ziu];
        if(zombies_flags[zi] & ZOMBIE_FLAG_EXISTS) {
            queue_draw_sprite_frame(zombie_sprite, zombies_x[zi], zombies_y[zi], zombies_frame[zi], zombies_flip[zi]);
        }
    }
}