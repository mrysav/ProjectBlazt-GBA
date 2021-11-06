#include <gba.h>

#include "player_object.h"

void PlayerObject::load(OBJATTR *spriteAttribs) { sprite.load(spriteAttribs); }

void PlayerObject::update(u16 counter, PlayerInputComponent &input) {
  physics.update(input, position);
  sprite.update(counter, position);
}
