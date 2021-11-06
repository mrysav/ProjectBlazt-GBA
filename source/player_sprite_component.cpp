#include <gba.h>

#include "im_spr_dangerous_dave.h"
#include "player_sprite_component.h"

// CENTER_PLAYER_SCREEN_X = (SCREEN_WIDTH / 2) - (PLAYER_SPRITE_WIDTH / 2);
#define CENTER_PLAYER_SCREEN_X 112
// CENTER_PLAYER_SCREEN_Y = SCREEN_HEIGHT - (PLAYER_SPRITE_HEIGHT * 3);
#define CENTER_PLAYER_SCREEN_Y 112

// Spritesheet information
#define PLAYER_SPRITE_HEIGHT 16
#define PLAYER_SPRITE_WIDTH 16

// Hitbox info
#define HITBOX_WIDTH 10
#define HITBOX_OFF_X 3
#define HITBOX_HEIGHT 16
#define HITBOX_OFF_Y 0

#define WALK_SEQ_LEN 4
const int walk_seq[WALK_SEQ_LEN] = {0, 1, 0, -1};

// Sprite IDs + animations
#define JUMPING_RIGHT 24
#define STANDING_RIGHT 8
#define WALKING_RIGHT 8
#define JUMPING_LEFT 32
#define STANDING_LEFT 48
#define WALKING_LEFT 48

PlayerSpriteComponent::PlayerSpriteComponent(OBJATTR *attribs) {

  // TODO: Support palette and gfx offsets for loading tiles in
  dmaCopy(im_spr_dangerous_davePal, SPRITE_PALETTE,
          im_spr_dangerous_davePalLen);
  dmaCopy(im_spr_dangerous_daveTiles, SPRITE_GFX,
          im_spr_dangerous_daveTilesLen);

  _position = Rectangle(0, 0, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT);
  _hitbox = Rectangle(HITBOX_OFF_X, HITBOX_OFF_Y, HITBOX_WIDTH, HITBOX_HEIGHT);

  spriteAttribs = attribs;

  /* set up the first attribute */
  spriteAttribs->attr0 = CENTER_PLAYER_SCREEN_Y | /* y coordinate */
                         (0 << 8) |               /* rendering mode */
                         (0 << 10) |              /* gfx mode */
                         (0 << 12) |              /* mosaic */
                         (1 << 13) |              /* color mode, 0:16, 1:256 */
                         (OBJ_SQUARE);            /* shape */

  /* set up the second attribute */
  spriteAttribs->attr1 = CENTER_PLAYER_SCREEN_X | /* x coordinate */
                         (0 << 9) |               /* affine flag */
                         (0 << 12) |              /* horizontal flip flag */
                         (0 << 13) |              /* vertical flip flag */
                         (ATTR1_SIZE_16);         /* size */

  /* setup the second attribute */
  spriteAttribs->attr2 = 0 |         // tile index */
                         (0 << 10) | // priority */
                         (0 << 12);  // palette bank (only 16 color)*/
}

void PlayerSpriteComponent::update(u16 counter, PositionComponent &position) {
  _position.set_x(position.get_x());
  _position.set_y(position.get_y());
  _hitbox.set_x(position.get_x() + HITBOX_OFF_X);
  _hitbox.set_y(position.get_y() + HITBOX_OFF_Y);

  // only animate once every 8 frames
  if (counter > (lastUpdate + 16)) {

    if (jumping) {
      firstFrame = facingLeft ? JUMPING_LEFT : JUMPING_RIGHT;
      animFrame = 0;
    } else if (moving) {
      firstFrame = facingLeft ? WALKING_LEFT : WALKING_RIGHT;
      animSeq = animSeq + 1;
      if (animSeq >= WALK_SEQ_LEN) {
        animSeq = 0; // avoid modulo
      }
      animFrame = walk_seq[animSeq];
    } else {
      firstFrame = facingLeft ? STANDING_LEFT : STANDING_RIGHT;
      animFrame = 0;
    }

    spriteAttribs->attr2 = firstFrame + (animFrame * 8);

    lastUpdate = counter;
  }
}