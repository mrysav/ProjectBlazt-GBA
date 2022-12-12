#pragma once

#include "position_component.h"
#include "pullrequest_sprite_component.h"

class PullRequestObject {
public:
  void load(OBJATTR *spriteAttribs, int start_x, int start_y);
  void update(u16 &counter);
  void move(int scroll_x, int scroll_y, Rectangle &viewport);

  bool &active() { return this->_active; }
  void set_active(bool active) { this->_active = active; }
  Rectangle &position() { return this->start_position; }

private:
  PullRequestSpriteComponent sprite;

  // position on screen, in relation to the level
  Rectangle _position;

  // initial starting position of the object
  // which is really the only logical position we care about
  Rectangle start_position;

  bool _active = true;
};
