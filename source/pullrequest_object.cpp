#include <gba.h>

#include "level_object.h"
#include "pullrequest_object.h"

void PullRequestObject::load(OBJATTR *spriteAttribs, int start_x, int start_y) {

  _position.set_x(start_x);
  _position.set_y(start_y);
  _position.set_height(16);
  _position.set_width(16);

  start_position.set_x(start_x);
  start_position.set_y(start_y);
  start_position.set_height(16);
  start_position.set_width(16);

  sprite.load(spriteAttribs, _position);
}

void PullRequestObject::update(u16 &counter) { sprite.animate(counter); }

void PullRequestObject::move(int scroll_x, int scroll_y, Rectangle &viewport) {
  _position.set_x(_position.x() + scroll_x);
  _position.set_y(_position.y() + scroll_y);

  if (_active && start_position.x() < viewport.x() + viewport.width() + 17 &&
      start_position.x() > viewport.x() - 17 &&
      start_position.y() < viewport.y() + viewport.height() + 17 &&
      start_position.y() > viewport.y() - 17) {
    sprite.update(_position);
  } else {
    sprite.hide();
  }
}
