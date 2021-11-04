#pragma once

#include "player_input_component.h"
#include "position_component.h"

class PhysicsComponent {
public:
  void update(PlayerInputComponent &input, PositionComponent &position);

private:
  int x_velocity;
  int y_velocity;
  int x_accel;
  int y_accel;
};
