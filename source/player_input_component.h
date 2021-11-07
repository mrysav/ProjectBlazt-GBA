#pragma once

#include <gba.h>

class PlayerInputComponent {
public:
  void update();

  bool down(u16 keys);
  bool up(u16 keys);
  bool held(u16 keys);

private:
  static u16 _last_keys;
  static u16 _keys;
};
