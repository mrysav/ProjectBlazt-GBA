#pragma once

#include "player_input_component.h"
#include "player_sprite_component.h"
#include "position_component.h"

class PhysicsComponent {
public:
  void update(PlayerInputComponent &input, PositionComponent &position,
              PlayerSpriteComponent &sprite, LevelObject &level);

private:
  int x_velocity;
  int y_velocity;
  int x_accel;
  int y_accel;
};
