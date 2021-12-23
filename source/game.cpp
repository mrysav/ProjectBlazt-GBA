#include <gba.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "game.h"

// Main tile palette
#include "im_bg_basic.h"

// Maps
#include "level_1.h"
#include "map_mario_1.h"

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

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define GRAVITY 4
#define RUN_VEL 2
#define JUMP_VEL -12
#define MAX_NPC_COUNT 50

using namespace std;

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

  player.load(&oam_object_backbuffer[0]);

  level.load(im_bg_basicPal, im_bg_basicPalLen, im_bg_basicTiles,
             im_bg_basicTilesLen, LEVEL_1_HEIGHT, LEVEL_1_WIDTH,
             im_bg_basicMetaTiles, LEVEL_1_BG0, LEVEL_1_BG1, LEVEL_1_BG2);
}

void GameState::unload() { clear_sprites(); }

StateType GameState::update() {
  input.update();
  level.update(input);
  player.update(counter, input, level);

  OAM[0] = oam_object_backbuffer[0];

  counter++;

  return GAME;
}
