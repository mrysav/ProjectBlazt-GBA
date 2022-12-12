#pragma once

#include "position_component.h"
#include "pullrequest_sprite_component.h"

class PullRequestObject {
public:
  void load(OBJATTR *spriteAttribs, int start_x, int start_y);
  void update(u16 &counter);
  void move(int scroll_x, int scroll_y, Rectangle &viewport);

private:
  PullRequestSpriteComponent sprite;
  // position screen, in relation to the level
  PositionComponent position;
  // initial starting position of the object
  PositionComponent start_position;
};
