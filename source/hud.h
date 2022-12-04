#pragma once

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
};
