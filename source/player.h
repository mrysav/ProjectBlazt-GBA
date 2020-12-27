# pragma once

#include <stdbool.h>
#include <stdint.h>

#include <gba.h>

#include "util.h"

// TODO: Maybe define spritesheet here?

typedef struct Player {
    OBJATTR* spriteAttribs;

    uint_fast16_t firstFrame;
    uint_fast16_t animFrame;
    uint_fast16_t framesInAir;

    bool isJumping;
    bool facingLeft;
    bool isMoving;

    Vec velocity;
    Vec position;
    Rect hitbox;
} Player;

void player_loadSprites();

void player_init(Player* p, OBJATTR* attribs);

void player_destroy(Player* player);

void player_tick(Player* player, int_fast16_t delay);

void player_setPosition(Player* this, int_fast16_t x, int_fast16_t y);

// Spritesheet information
#define PLAYER_SPRITE_HEIGHT 16
#define PLAYER_SPRITE_WIDTH 16

// extern const u16 CENTER_PLAYER_SCREEN_X = (SCREEN_WIDTH / 2) - (PLAYER_SPRITE_WIDTH / 2);
// extern const u16 CENTER_PLAYER_SCREEN_Y = SCREEN_HEIGHT - (PLAYER_SPRITE_HEIGHT * 3);
#define CENTER_PLAYER_SCREEN_X 112
#define CENTER_PLAYER_SCREEN_Y 112

// extern const u16 MAX_PLAYER_SCREEN_X = SCREEN_WIDTH - PLAYER_SPRITE_WIDTH;
// extern const u16 MAX_PLAYER_SCREEN_Y = SCREEN_HEIGHT - PLAYER_SPRITE_HEIGHT;
#define MAX_PLAYER_SCREEN_X 224
#define MAX_PLAYER_SCREEN_Y 144

Vec player_getScreenPos(Player* this);
void player_setScreenX(Player* this, u16 x);
void player_setScreenY(Player* this, u16 y);
