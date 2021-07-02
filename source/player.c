#include "player.h"

#include <gba.h>
#include <stdint.h>

#include "im_spr_dangerous_dave.h"

#define WALK_SEQ_LEN 4
const int walk_seq[WALK_SEQ_LEN] = {0, 1, 0, -1};

// Sprite IDs + animations
#define JUMPING_RIGHT 24
#define STANDING_RIGHT 8
#define WALKING_RIGHT 8
#define JUMPING_LEFT 32
#define STANDING_LEFT 48
#define WALKING_LEFT 48

// Hitbox info
#define HITBOX_WIDTH 10
#define HITBOX_OFF_X 3
#define HITBOX_HEIGHT 16
#define HITBOX_OFF_Y 0

void player_loadSprites()
{
    dmaCopy(im_spr_dangerous_davePal, SPRITE_PALETTE, im_spr_dangerous_davePalLen);
    dmaCopy(im_spr_dangerous_daveTiles, SPRITE_GFX, im_spr_dangerous_daveTilesLen);
}

void player_init(Player *p, OBJATTR *attribs)
{
    p->spriteAttribs = attribs;

    /* set up the first attribute */
    p->spriteAttribs->attr0 = CENTER_PLAYER_SCREEN_Y | /* y coordinate */
                              (0 << 8) |               /* rendering mode */
                              (0 << 10) |              /* gfx mode */
                              (0 << 12) |              /* mosaic */
                              (1 << 13) |              /* color mode, 0:16, 1:256 */
                              (OBJ_SQUARE);            /* shape */

    /* set up the second attribute */
    p->spriteAttribs->attr1 = CENTER_PLAYER_SCREEN_X | /* x coordinate */
                              (0 << 9) |               /* affine flag */
                              (0 << 12) |              /* horizontal flip flag */
                              (0 << 13) |              /* vertical flip flag */
                              (ATTR1_SIZE_16);         /* size */

    /* setup the second attribute */
    p->spriteAttribs->attr2 = 0 |         // tile index */
                              (0 << 10) | // priority */
                              (0 << 12);  // palette bank (only 16 color)*/

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

void player_destroy(Player *player)
{
    player->isJumping = false;
    player->firstFrame = 0;
    player->animFrame = 0;
    player->isMoving = false;
}

void player_setPosition(Player *this, int x, int y)
{
    this->position.x = x;
    this->position.y = y;
    this->hitbox.x = this->position.x + HITBOX_OFF_X;
    this->hitbox.y = this->position.y + HITBOX_OFF_Y;
}

void player_tick(Player *player, int delay)
{
    // only animate once every 8th frame
    if ((delay % 16) > 0)
    {
        return;
    }

    static int anim_seq;

    if (player->isMoving)
    {
        // if (player->isJumping) {
        //     player->firstFrame = player->facingLeft ? JUMPING_LEFT : JUMPING_RIGHT;
        //     player->animFrame = 0;
        // } else {
        player->firstFrame = player->facingLeft ? WALKING_LEFT : WALKING_RIGHT;
        anim_seq = ++anim_seq % WALK_SEQ_LEN;
        player->animFrame = walk_seq[anim_seq];
        // }
    }
    else
    {
        player->firstFrame = player->facingLeft ? STANDING_LEFT : STANDING_RIGHT;
        player->animFrame = 0;
    }

    player->spriteAttribs->attr2 = player->firstFrame + (player->animFrame * 8);
}

Vec player_getScreenPos(Player *this)
{
    Vec vec;
    // Bitwise AND to get the lowest 8 bits
    vec.y = this->spriteAttribs->attr0 & 0x00FF;
    // This attribute uses the lower 9 bits
    vec.x = this->spriteAttribs->attr1 & 0x01FF;
    return vec;
}

void player_setScreenX(Player *this, u16 x)
{
    this->spriteAttribs->attr1 = (this->spriteAttribs->attr1 & 0xFE00) | (0x01FF & x);
}

void player_setScreenY(Player *this, u16 y)
{
    this->spriteAttribs->attr0 = (this->spriteAttribs->attr0 & 0xFF00) | (0x00FF & y);
}
