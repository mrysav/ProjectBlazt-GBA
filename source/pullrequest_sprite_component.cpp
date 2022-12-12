#include <gba.h>

#include "game_sprites.h"
#include "pullrequest_sprite_component.h"

// Spritesheet information
#define SPRITE_HEIGHT 16
#define SPRITE_WIDTH 16

// Hitbox info
#define HITBOX_WIDTH 16
#define HITBOX_OFF_X 0
#define HITBOX_HEIGHT 16
#define HITBOX_OFF_Y 0

#define ANIM_SEQ_LEN 6
const int anim_seq[ANIM_SEQ_LEN] = {0, 1, 2, 3, 2, 1};

// Sprite IDs + animations
#define PR_NEUTRAL 64

void PullRequestSpriteComponent::load(OBJATTR *attribs, Rectangle &position) {
  _position =
      Rectangle(position.x(), position.y(), SPRITE_HEIGHT, SPRITE_WIDTH);

  spriteAttribs = attribs;

  /* set up the first attribute */
  spriteAttribs->attr0 = 0 |           /* y coordinate */
                         (0 << 8) |    /* rendering mode */
                         (0 << 10) |   /* gfx mode */
                         (0 << 12) |   /* mosaic */
                         (1 << 13) |   /* color mode, 0:16, 1:256 */
                         (OBJ_SQUARE); /* shape */

  /* set up the second attribute */
  spriteAttribs->attr1 = 0 |              /* x coordinate */
                         (0 << 9) |       /* affine flag */
                         (0 << 12) |      /* horizontal flip flag */
                         (0 << 13) |      /* vertical flip flag */
                         (ATTR1_SIZE_16); /* size */

  /* setup the second attribute */
  spriteAttribs->attr2 = PR_NEUTRAL | // tile index */
                         (0 << 10) |  // priority */
                         (0 << 12);   // palette bank (only 16 color)*/
}

void PullRequestSpriteComponent::animate(u16 &counter) {
  if (counter > (lastUpdate + 6)) {
    animSeq = (animSeq + 1) % ANIM_SEQ_LEN;
    animFrame = anim_seq[animSeq];
    lastUpdate = counter;
  }

  spriteAttribs->attr2 = PR_NEUTRAL + (animFrame * 8);
}
void PullRequestSpriteComponent::update(Rectangle &position) {
  _position.set_x(position.x());
  _position.set_y(position.y());

  /* set up the first attribute */
  spriteAttribs->attr0 = OBJ_Y(position.y()) | /* y coordinate */
                         (0 << 8) |            /* rendering mode */
                         (0 << 10) |           /* gfx mode */
                         (0 << 12) |           /* mosaic */
                         (1 << 13) |           /* color mode, 0:16, 1:256 */
                         (OBJ_SQUARE);         /* shape */

  /* set up the second attribute */
  spriteAttribs->attr1 = OBJ_X(position.x()) | /* x coordinate */
                         (0 << 9) |            /* affine flag */
                         (0 << 12) |           /* horizontal flip flag */
                         (0 << 13) |           /* vertical flip flag */
                         (ATTR1_SIZE_16);      /* size */
}

void PullRequestSpriteComponent::hide() {
  spriteAttribs->attr0 = SCREEN_HEIGHT;
  spriteAttribs->attr1 = SCREEN_WIDTH;
}
