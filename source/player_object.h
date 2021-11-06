#pragma once

#include "physics_component.h"
#include "player_input_component.h"
#include "player_sprite_component.h"
#include "position_component.h"

class PlayerObject {
public:
  void load(OBJATTR *spriteAttribs);
  void update(u16 counter, PlayerInputComponent &input);

private:
  PhysicsComponent physics;
  PlayerSpriteComponent sprite;
  PositionComponent position;
};
