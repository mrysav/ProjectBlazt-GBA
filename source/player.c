#include <stdint.h>

#include <gba.h>

#include "dangerous_dave.h"

#include "player.h"

#define WALK_SEQ_LEN 4
const int_fast16_t walk_seq[WALK_SEQ_LEN] = {0, 1, 0, -1};

// Sprite IDs + animations
#define JUMPING_RIGHT 24
#define STANDING_RIGHT 8
#define WALKING_RIGHT 8
#define JUMPING_LEFT 32
#define STANDING_LEFT 48
#define WALKING_LEFT 48

// Spritesheet information
#define SPRITE_HEIGHT 16
#define SPRITE_WIDTH 16
#define SHEET_HEIGHT 2
#define SHEET_WIDTH 4

// Hitbox info
#define HITBOX_WIDTH 10
#define HITBOX_OFF_X 3
#define HITBOX_HEIGHT 16
#define HITBOX_OFF_Y 0

void loadPlayerSpriteSheet() {
    // TODO: MAP might not be right here
    memcpy(&MAP[4][0], dangerous_daveTiles, dangerous_daveTilesLen);
    memcpy(SPRITE_PALETTE, dangerous_davePal, dangerous_davePalLen);
}

void player_init(Player* p, OBJATTR* attribs) {
    loadPlayerSpriteSheet();

    p->spriteAttribs = attribs;
    p->firstFrame = 0;
    p->animFrame = 0;
    p->framesInAir = 0;

    p->isJumping = false;
    p->facingLeft = false;
    p->isMoving = false;

    p->velocity.x = 0;
    p->velocity.y = 0;

    p->position.x = 0;
    p->position.y = 0;

    p->hitbox.x = p->position.x + HITBOX_OFF_X;
    p->hitbox.y = p->position.y + HITBOX_OFF_Y;
    p->hitbox.width = HITBOX_WIDTH;
    p->hitbox.height = HITBOX_HEIGHT;
}

void player_destroy(Player* player) {
    player->isJumping = false;
    player->firstFrame = 0;
    player->animFrame = 0;
    player->isMoving = false;
}

void player_setPosition(Player* this, int_fast16_t x, int_fast16_t y) {
    this->position.x = x;
    this->position.y = y;
    this->hitbox.x = this->position.x + HITBOX_OFF_X;
    this->hitbox.y = this->position.y + HITBOX_OFF_Y;
}

void player_tick(Player* player, int_fast16_t delay) {
    // only animate once every 8th frame
    if ((delay % 8) > 0) {
        return;
    }

    static int_fast16_t anim_seq;

    if (player->isMoving) {
        if (player->isJumping) {
            player->firstFrame = player->facingLeft ? JUMPING_LEFT : JUMPING_RIGHT;
            player->animFrame = 0;
        } else {
            player->firstFrame = player->facingLeft ? WALKING_LEFT : WALKING_RIGHT;
            anim_seq = ++anim_seq % WALK_SEQ_LEN;
            player->animFrame = walk_seq[anim_seq];
        }
    } else {
        player->firstFrame = player->facingLeft ? STANDING_LEFT : STANDING_RIGHT;
        player->animFrame = 0;
    }

    player->spriteAttribs->attr2 = player->firstFrame + (player->animFrame * 8);
}

void player_draw(Player* player, Rect* camera) {
    // int_fast16_t x1 = player->position.x - camera->x;
    // int_fast16_t y1 = player->position.y - camera->y;

    // int_fast16_t spriteId = player->firstFrame + player->animFrame;
    // int_fast16_t tx = spriteId % SHEET_WIDTH * SPRITE_WIDTH;
    // int_fast16_t ty = spriteId / SHEET_WIDTH * SPRITE_HEIGHT;

    // al_draw_bitmap_region(player_sheet, tx, ty, SPRITE_WIDTH, SPRITE_HEIGHT, x1, y1, 0);
}
