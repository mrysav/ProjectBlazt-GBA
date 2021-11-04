#include <gba.h>

#include "player_object.h"

void PlayerObject::update(u16 counter) {
  input.update(counter);
  physics.update(input, position);
  sprite.update(counter, position);
}
