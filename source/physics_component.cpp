#include <gba_input.h>

#include "physics_component.h"
#include "player_input_component.h"
#include "position_component.h"

void PhysicsComponent::update(PlayerInputComponent &input,
                              PositionComponent &position) {
  if (input.held(KEY_RIGHT) && !input.held(KEY_LEFT)) {
    x_velocity = 1;
  } else if (input.held(KEY_LEFT) && !input.held(KEY_RIGHT)) {
    x_velocity = -1;
  }

  if (y_velocity == 0 && input.down(KEY_A)) {
    y_velocity = -4;
  }

  y_velocity += 1;
}
