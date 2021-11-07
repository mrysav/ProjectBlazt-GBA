#pragma once

#include "position_component.h"
#include "rectangle.h"

class PlayerSpriteComponent {
public:
  void load(OBJATTR *attribs, PositionComponent &PositionComponent);
  void update(u16 counter, PositionComponent &position);

  Rectangle &hitbox() { return _hitbox; }

  void setFacingLeft(bool facingLeft) { facingLeft = facingLeft; }
  void setJumping(bool jumping) { jumping = jumping; }
  void setMoving(bool moving) { moving = moving; }

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
