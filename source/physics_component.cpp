#include <gba_input.h>

#include "level_object.h"
#include "physics_component.h"
#include "player_input_component.h"
#include "position_component.h"

void PhysicsComponent::update(PlayerInputComponent &input,
                              PositionComponent &position,
                              PlayerSpriteComponent &sprite,
                              LevelObject &level) {
  if (input.down(KEY_RIGHT)) {
    x_velocity = 2;
  }

  if (input.down(KEY_LEFT)) {
    x_velocity = -2;
  }

  if (!(input.down(KEY_LEFT & KEY_RIGHT))) {
    if (x_velocity < 0)
      sprite.setFacingLeft(true);
    if (x_velocity > 0)
      sprite.setFacingLeft(false);
  }

  if (y_velocity == 0 && input.down(KEY_A)) {
    y_velocity = -2;
  }

  if (y_velocity < 5) {
    y_velocity += 1;
  }

  auto res = level.resolve_collision(sprite.hitbox(), x_velocity, y_velocity);

  sprite.setJumping(res.y_dist != 0);

  // collisions happened
  if (res.x_dist != x_velocity) {
    x_velocity = 0;
  }
  if (res.y_dist != y_velocity) {
    y_velocity = 0;
  }

  position.set_x(position.get_x() + res.x_dist);
  position.set_y(position.get_y() + res.y_dist);

  level.move_viewport(res.x_dist, res.y_dist);
}
