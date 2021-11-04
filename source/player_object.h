#pragma once

#include "physics_component.h"
#include "player_input_component.h"
#include "player_sprite_component.h"
#include "position_component.h"

class PlayerObject {
public:
  PlayerObject(OBJATTR *spriteAttribs)
      : sprite(PlayerSpriteComponent(spriteAttribs)) {}

  void update(u16 counter);

private:
  PhysicsComponent physics = PhysicsComponent();
  PlayerInputComponent input = PlayerInputComponent();
  PlayerSpriteComponent sprite;
  PositionComponent position = PositionComponent();
};
