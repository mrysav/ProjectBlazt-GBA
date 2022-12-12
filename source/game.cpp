#include <gba.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game.h"

// Main tile palette
#include "im_bg_basic.h"

// Sprites and palette
#include "game_sprites.h"

// Maps
#include "level_1.h"
#include "map_mario_1.h"

// Game objects
#include "hud.h"

// Utilities
#include "display.h"
#include "util.h"

// Components
#include "physics_component.h"
#include "player_input_component.h"
#include "player_sprite_component.h"
#include "position_component.h"

// Object classes
#include "level_object.h"
#include "player_object.h"

void clear_sprites() {
  for (int i = 0; i < 128; i++) {
    OAM[i].attr0 = SCREEN_HEIGHT;
    OAM[i].attr1 = SCREEN_WIDTH;
  }
}

OBJATTR oam_object_backbuffer[128];

GameState::GameState() {}

void GameState::load() {

  clear_sprites();

  // TODO: Support palette and gfx offsets for loading tiles in
  dmaCopy(game_spritesPal, SPRITE_PALETTE, game_spritesPalLen);
  dmaCopy(game_spritesTiles, SPRITE_GFX, game_spritesTilesLen);

  player.load(&oam_object_backbuffer[0]);

  pull_requests[0].load(&oam_object_backbuffer[1], 2 * 16, 2 * 16);
  pull_requests[1].load(&oam_object_backbuffer[2], 8 * 16, 2 * 16);
  pull_requests[2].load(&oam_object_backbuffer[3], 5 * 16, 14 * 16);
  pull_requests[3].load(&oam_object_backbuffer[4], 2 * 16, 2 * 16);
  pull_requests[4].load(&oam_object_backbuffer[5], 2 * 16, 2 * 16);
  pull_requests[5].load(&oam_object_backbuffer[6], 2 * 16, 2 * 16);
  pull_requests[6].load(&oam_object_backbuffer[7], 2 * 16, 2 * 16);
  pull_requests[7].load(&oam_object_backbuffer[8], 2 * 16, 2 * 16);

  level.load(im_bg_basicPal, im_bg_basicPalLen, im_bg_basicTiles,
             im_bg_basicTilesLen, LEVEL_1_HEIGHT, LEVEL_1_WIDTH,
             im_bg_basicMetaTiles, LEVEL_1_BG0, LEVEL_1_BG1, pull_requests, 8);
}

void GameState::unload() { clear_sprites(); }

StateType GameState::update() {
  input.update();
  level.update(counter, input);
  player.update(counter, input, level);

  for (int p = 0; p < 8; p++) {
    pull_requests[p].update(counter);
  }

  for (int i = 0; i < 9; i++) {
    OAM[i] = oam_object_backbuffer[i];
  }

  counter++;

  return GAME;
}
