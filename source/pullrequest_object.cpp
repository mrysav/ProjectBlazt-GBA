#include <gba.h>

#include "level_object.h"
#include "pullrequest_object.h"

void PullRequestObject::load(OBJATTR *spriteAttribs, int start_x, int start_y) {

  position.set_x(start_x);
  position.set_y(start_y);

  start_position.set_x(start_x);
  start_position.set_y(start_y);

  sprite.load(spriteAttribs, position);
}

void PullRequestObject::update(u16 &counter) { sprite.animate(counter); }

void PullRequestObject::move(int scroll_x, int scroll_y, Rectangle &viewport) {
  position.set_x(position.get_x() + scroll_x);
  position.set_y(position.get_y() + scroll_y);

  if (start_position.get_x() < viewport.x() + viewport.width() + 17 &&
      start_position.get_x() > viewport.x() - 17 &&
      start_position.get_y() < viewport.y() + viewport.height() + 17 &&
      start_position.get_y() > viewport.y() - 17) {
    sprite.update(position);
  } else {
    sprite.hide();
  }
}
