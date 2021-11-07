#include <gba.h>

#include "level_object.h"
#include "player_object.h"

// CENTER_PLAYER_SCREEN_X = (SCREEN_WIDTH / 2) - (PLAYER_SPRITE_WIDTH / 2);
#define CENTER_PLAYER_SCREEN_X 112
// CENTER_PLAYER_SCREEN_Y = SCREEN_HEIGHT - (PLAYER_SPRITE_HEIGHT * 3);
#define CENTER_PLAYER_SCREEN_Y 112

void PlayerObject::load(OBJATTR *spriteAttribs) {

  position.set_x(CENTER_PLAYER_SCREEN_X);
  position.set_y(CENTER_PLAYER_SCREEN_Y);

  sprite.load(spriteAttribs, position);
}

void PlayerObject::update(u16 &counter, PlayerInputComponent &input,
                          LevelObject &level) {
  physics.update(input, position, sprite, level);
  sprite.update(counter, position);
}
