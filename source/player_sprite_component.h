#pragma once

#include "position_component.h"
#include "rectangle.h"

class PlayerSpriteComponent {
public:
  PlayerSpriteComponent(OBJATTR *attribs);

  void update(u16 counter, PositionComponent &position);

  Rectangle &hitbox() { return _hitbox; }

private:
  OBJATTR *spriteAttribs;

  u16 firstFrame = 0;
  u16 animFrame = 0; // actual frame in sprite memory we're on
  u16 animSeq = 0;   // id of frame in walk sequence

  bool jumping = false;
  bool moving = false;
  bool facingLeft = false;

  Rectangle _position;
  Rectangle _hitbox;

  u16 lastUpdate = 0;
};
