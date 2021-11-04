#include <gba_input.h>

#include "player_input_component.h"

u16 PlayerInputComponent::_last_keys = 0;
u16 PlayerInputComponent::_keys = 0;
u16 PlayerInputComponent::_lastUpdate = 0;

void PlayerInputComponent::update(u16 counter) {
  if (counter == _lastUpdate)
    return;

  _last_keys = _keys;
  _keys = keysDown();
  _lastUpdate = counter;
}

bool PlayerInputComponent::down(u16 keys) { return keys & _keys; }

bool PlayerInputComponent::up(u16 keys) {
  return (keys & _last_keys) && !(keys & _keys);
}

bool PlayerInputComponent::held(u16 keys) {
  return (keys & _keys) && (keys & _last_keys);
}
