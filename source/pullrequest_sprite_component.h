#pragma once

#include <gba.h>

#include "position_component.h"
#include "rectangle.h"

class PullRequestSpriteComponent {
public:
  void load(OBJATTR *attribs, PositionComponent &PositionComponent);
  void update(PositionComponent &position);
  void animate(u16 &counter);
  void hide();

  Rectangle &position() { return _position; }

private:
  OBJATTR *spriteAttribs;

  u16 animFrame = 0; // actual frame in sprite memory we're on
  u16 animSeq = 0;   // id of frame in walk sequence

  Rectangle _position;

  u16 lastUpdate = 0;
};
