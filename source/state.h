#pragma once

#include <stdint.h>

typedef enum State { MENU, GAME, EXIT } State;

typedef struct GameState {
  void (*loadResources)();
  State (*processInput)(uint16_t keys);
  void (*updateDisplay)();
  void (*unloadResources)();
} GameState;
