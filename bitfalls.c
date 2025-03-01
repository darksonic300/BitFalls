#include <stdio.h>
#include "gamecharacter.c"

// Sprite includes
#include "sprites/player.c"
#include "sprites/banana.c"
#include "sprites/apple.c"
#include "sprites/medal.c"
#include "sprites/spike.c"

#include "sprites/scrTitle_data.c"
#include "sprites/scrTitle_map.c"

// Game Boy includes
#include "../gbdk/include/gb/gb.h"

// IMPORTANT - 0 = true, 1 = false

// Prototypes
void setup_player(void);
void setup_bananas(UINT8 number);
void setup_apples(UINT8 number);
void setup_medal(void);
void setup_spikes(UINT8 number);
void setup_game_character(gamecharacter* character, UINT8 x, UINT8 y, UINT8 width, UINT8 height, UINT8 spriteid, UINT8 tile);
UBYTE is_player_within_bounds(gamecharacter* player);
UBYTE has_collision_happened(gamecharacter* first, gamecharacter* second);
void setup_sound(void);
void play_button_sound(void);
void play_moving_sound(void);
void play_death_sound(void);
void move_character(gamecharacter* character, UINT8 x, UINT8 y);
void game_over(UBYTE win);
void performant_delay(UINT8 time);

// Global Structs
gamecharacter basket_player;
gamecharacter banana[3];
gamecharacter apple[2];
gamecharacter medal;
gamecharacter spike[2];

UINT8 main(void) {
    UINT8 tick_count = 0;

    DISPLAY_ON;
    setup_sound();

    set_bkg_data(0, 156, scrTitle_data);
    set_bkg_tiles(0, 0, 20, 18, scrTitle_map);

    SHOW_BKG;
    waitpad(J_START);
    play_button_sound();
    HIDE_BKG;

    SHOW_SPRITES;
    set_sprite_data(0, 0, Player);

    setup_player();
    setup_bananas(3);

    while (1) {
        if (joypad() & J_LEFT) {
            basket_player.x -= 4;
            if (is_player_within_bounds(&basket_player) == 0) {
                move_character(&basket_player, basket_player.x, basket_player.y);
            } else basket_player.x += 4;
        }
        if (joypad() & J_RIGHT) {
            basket_player.x += 4;
            if (is_player_within_bounds(&basket_player) == 0) {
                move_character(&basket_player, basket_player.x, basket_player.y);
            } else basket_player.x -= 4;
        }



        performant_delay(5); //10

        tick_count++;
    }
}

void performant_delay(UINT8 time) {
    for (UINT8 i = 0; i < time; i++) {
        wait_vbl_done();
    }
}

void setup_sound(void) {
    // Sound Registers. This is their specific order
    NR52_REG = 0x80;    // Turns on sound
    NR50_REG = 0x77;    // Sets the volume to max
    NR51_REG = 0xFF;    // Selects all channels to use
}

void play_shooting_sound(void) {
    NR10_REG = 0x16;    // See https://www.youtube.com/watch?v=psCxZr9iDck
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;
}

void play_button_sound(void) {
    NR10_REG = 0x54;
    NR11_REG = 0x81;
    NR12_REG = 0x43;
    NR13_REG = 0x73;
    NR14_REG = 0x86;
}

void play_moving_sound(void) {
    NR41_REG = 0x3F;
    NR42_REG = 0x31;
    NR43_REG = 0x70;
    NR44_REG = 0x80;
}

void play_death_sound(void) {
    NR41_REG = 0x20;
    NR42_REG = 0x68;
    NR43_REG = 0x6F;
    NR44_REG = 0xC0;
}

void setup_player(void) {
    setup_game_character(&basket_player, 84, 136, 8, 8, 0, 0);
}

void setup_bananas(UINT8 number) {
    UINT8 i;
    for (i = 0; i < number; i++) {
        setup_game_character(&banana[i], 36 + i * 16, 32, 8, 8, i + 2, 3);
    }
}

void setup_game_character(gamecharacter* character, UINT8 x, UINT8 y, UINT8 width, UINT8 height, UINT8 spriteid, UINT8 tile) {
    character->x = x;
    character->y = y;
    character->width = width;
    character->height = height;

    set_sprite_tile(spriteid, tile);
    character->spriteid = spriteid;

    move_character(character, x, y);
}

UBYTE is_player_within_bounds(gamecharacter* player) {
    return player->x > 152 || player->x < 16 ? 1 : 0;
}

UBYTE has_collision_happened(gamecharacter* first, gamecharacter* second) {
    if ((first->x >= second->x - second->width && first->x  <= second->x &&
        first->y <= second->y + second->height && first->y >= second->y) ||
        (second->x >= first->x - first->width && second->x  <= first->x &&
        second->y <= first->y + first->height && second->y >= first->y)) {
            return 0;
    }
    return 1;
}

void move_character(gamecharacter* character, UINT8 x, UINT8 y) {
    move_sprite(character->spriteid, x, y);
}

void game_over(UBYTE win) {
    SHOW_BKG;
    HIDE_SPRITES;
    printf(" \n \n \n \n === Game  Over ===");
    if (win == 0) {
        printf("\n \n - Player Won");
    } else {
        printf("\n \n - Player Lost");
    }
}