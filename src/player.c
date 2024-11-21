#include "player.h"
#include "gt/gfx/sprites.h"
#include "gt/gfx/draw_queue.h"
#include "gt/input.h"
#include "gt/audio/music.h"

#include "gen/assets/player.h"
#include "gen/assets/playershoot.h"
#include "gen/assets/player/normal.json.h"
#include "gen/assets/audio.h"

SpriteSlot player_default_sprite;
SpriteSlot player_shooting_sprite;

char player_frame;
char player_subframe;
char player_subframe_count;
char player_direction;
char player_shooting;
char player_x, player_y;
char player_anim;

#define DIR_D  0
#define DIR_DL 1
#define DIR_UL 2
#define DIR_U  3
#define DIR_UR 4
#define DIR_DR 5
#define DIR_R  6
#define DIR_L  7
#define DIR_X 0xFF

#define PLAYER_ANIM_CYCLE 8
#define PLAYER_SUBFRAMES_IDLE 12
#define PLAYER_SUBFRAMES_ACTIVE 4

static const char direction_offsets[] = {0, 8, 16, 24, 32, 40, 48, 56};

#define DIR_MAP_CENTER 4
static const char direction_map[] = {
    DIR_UL, DIR_U, DIR_UR,
    DIR_L , DIR_X, DIR_R,
    DIR_DL, DIR_D, DIR_DR
};

void init_player() {
    player_default_sprite = allocate_sprite(&ASSET__player__normal_bmp_load_list);
    player_shooting_sprite = allocate_sprite(&ASSET__playershoot__shooting_bmp_load_list);
    set_sprite_frametable(player_default_sprite, &ASSET__player__normal_json);
    set_sprite_frametable(player_shooting_sprite, &ASSET__playershoot__shooting_json);

    player_frame = 0;
    player_subframe = 0;
    player_shooting = 0;
    player_x = 64;
    player_y = 64;
    player_direction = DIR_D;
    player_anim = 0;
    player_subframe_count = PLAYER_SUBFRAMES_IDLE;
}

void update_player() {
    static char i;

    if((++player_subframe) >= player_subframe_count) {
        player_subframe = 0;
        if((++player_frame) == PLAYER_ANIM_CYCLE) {
            player_frame = 0;
        }
    }

    i = DIR_MAP_CENTER;
    if(player1_buttons & INPUT_MASK_LEFT) {
        --i;
        --player_x;
    }
    if(player1_buttons & INPUT_MASK_RIGHT) {
        ++i;
        ++player_x;
    }
    if(player1_buttons & INPUT_MASK_UP) {
        i -= 3;
        --player_y;
    }
    if(player1_buttons & INPUT_MASK_DOWN) {
        i += 3;
        ++player_y;
    }

    if(player1_buttons & INPUT_MASK_A) {
        player_shooting = 1;
    } else {
        player_shooting = 0;
    }

    if(i != DIR_MAP_CENTER) {
        if(~player1_buttons & INPUT_MASK_A)
            player_direction = direction_map[i];
        player_anim = NORMAL_TAG_RUN_START;
    } else {
        player_anim = NORMAL_TAG_IDLE_START;
    }

    if(player_shooting && !player_subframe && (~player_frame & 1)) {
        play_sound_effect(ASSET__audio__shoot_sfx_ID, 2);
    }

    if(player_shooting || (i != DIR_MAP_CENTER)) {
        player_subframe_count = PLAYER_SUBFRAMES_ACTIVE;
    } else {
        player_subframe_count = PLAYER_SUBFRAMES_IDLE;
    }
}

void draw_player() {
    if(player_shooting) {
        queue_draw_sprite_frame(player_shooting_sprite, player_x, player_y, player_frame + player_anim + direction_offsets[player_direction], 0);
    } else {
        queue_draw_sprite_frame(player_default_sprite, player_x, player_y, player_frame + player_anim + direction_offsets[player_direction], 0);
    }
}