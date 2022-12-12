#include <gba_input.h>

#include "level_object.h"
#include "physics_component.h"
#include "player_input_component.h"
#include "position_component.h"

void PhysicsComponent::update(PlayerInputComponent &input,
                              PositionComponent &position,
                              PlayerSpriteComponent &sprite,
                              LevelObject &level) {
  if (input.down(KEY_RIGHT) && x_velocity < 2) {
    sprite.setFacingLeft(false);
    x_velocity += 1;
  }

  if (input.down(KEY_LEFT) && x_velocity > -2) {
    sprite.setFacingLeft(true);
    x_velocity -= 1;
  }

  if (!input.down(KEY_LEFT) && !input.down(KEY_RIGHT)) {
    if (x_velocity < 0) {
      x_velocity += 1;
    } else if (x_velocity > 0) {
      x_velocity -= 1;
    }
  }

  if (input.down(KEY_A) && !sprite.isJumping()) {
    sprite.setJumping(true);
    y_velocity = -11;
  }

  if (y_velocity < 2) {
    y_velocity += 1;
  }

  auto res = level.resolve_collision(sprite.hitbox(), x_velocity, y_velocity);

  // collisions happened
  if (res.x_dist != x_velocity) {
    x_velocity = 0;
  }
  if (res.y_dist != y_velocity) {
    if (sprite.isJumping() && y_velocity > res.y_dist) {
      sprite.setJumping(false);
    }
    y_velocity = 0;
  }

  sprite.setMoving(res.x_dist != 0 && !sprite.isJumping());

  position.set_x(position.get_x() + res.x_dist);
  position.set_y(position.get_y() + res.y_dist);

  level.move_viewport(res.x_dist, res.y_dist);
  level.update_collide(res);
}
