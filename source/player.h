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

void player_draw(Player* player, Rect* camera);

void player_setPosition(Player* this, int_fast16_t x, int_fast16_t y);
