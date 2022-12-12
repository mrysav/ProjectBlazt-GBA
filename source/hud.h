#pragma once

#include <gba.h>
#include <stdint.h>

#include "util.h"

#define HUD_LEN 600
#define HUD_HEIGHT 10
#define HUD_WIDTH 15

extern const uint16_t HUD_BG[150];

class HudComponent {
public:
  void initialize();
  void update(uint timer_value, uint score);
  u16 &timer() { return _timer; }

private:
  void update_tile(int x, int y, u16 tile);

  u16 *base_screenblock = (u16 *)SCREEN_BASE_BLOCK(12);

  u16 _timer = 0;
  u16 _score = 0;
};
