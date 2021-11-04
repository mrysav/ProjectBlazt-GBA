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

// #include "npc.h"
// #include "red_dude.h"

// #include "level.h"
// #include "underground_1.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

// const Level* ALL_LEVELS[3] = { &UNDERGROUND_1 };
// const Level* currentLevel;

#define GRAVITY 4
#define RUN_VEL 2
#define JUMP_VEL -12
#define MAX_NPC_COUNT 50

using namespace std;

// NPC enemies[MAX_NPC_COUNT] = {0};

// Set when loadResources() is called
uint map_total_width;
uint map_total_height;
uint max_move_x;
uint max_move_y;
uint max_camera_x;
uint max_camera_y;

// Rect camera = {-CENTER_PLAYER_SCREEN_X, -CENTER_PLAYER_SCREEN_Y,
// SCREEN_WIDTH, SCREEN_HEIGHT};

int numEnem = 0;

typedef struct level_data {
  uint height;
  uint width;
  uint len;

  const uint16_t *layer0;
  const uint16_t *layer1;
  const uint16_t *layer2;

} level_data;

level_data const MARIO_1_1 = {
    MAP_MARIO_1_HEIGHT, MAP_MARIO_1_WIDTH, MAP_MARIO_1_LEN,
    MAP_MARIO_1_BG0,    MAP_MARIO_1_BG1,   MAP_MARIO_1_BG2,
};

level_data const LEVEL_1_DBG = {
    LEVEL_1_HEIGHT, LEVEL_1_WIDTH, LEVEL_1_LEN,
    LEVEL_1_BG0,    LEVEL_1_BG1,   LEVEL_1_BG2,
};

level_data const *active_level;

void clear_sprites() {
  for (int i = 0; i < 128; i++) {
    OAM[i].attr0 = SCREEN_HEIGHT;
    OAM[i].attr1 = SCREEN_WIDTH;
  }
}

OBJATTR oam_object_backbuffer[128];

GameState::GameState() : player(PlayerObject(&oam_object_backbuffer[0])) {}

void GameState::load() {

  level =
      LevelObject(im_bg_basicPal, im_bg_basicPalLen, im_bg_basicTiles,
                  im_bg_basicTilesLen, LEVEL_1_HEIGHT, LEVEL_1_WIDTH,
                  im_bg_basicMetaTiles, LEVEL_1_BG0, LEVEL_1_BG1, LEVEL_1_BG2);

  // clear_sprites(); // ?
}

void GameState::unload() { clear_sprites(); }

// void spawnEnemy() {
//     static int_fast8_t delay = 0;
//     delay = (++delay) % 30;
//     if (delay > 0) {
//         return;
//     }

//     if (numEnem >= 10) {
//         return;
//     }
//     enemies[numEnem] = red_dude;
//     Vec32 pos = {0,0};
//     enemies[numEnem].init(&enemies[numEnem], pos);
//     numEnem++;
// }

// gravdata calc_collide(int xvel, int xedge, int x, int width, int yvel,
//                       int yedge, int y, int height, const u16 *map,
//                       u16 map_width, u16 map_height) {
//   gravdata res = {0};

//   if (xvel != 0) {
//     int tileX = (xedge + xvel) / TILE_WIDTH;
//     int topTileY = y / TILE_HEIGHT;
//     int botTileY = (y + height - 1) / TILE_HEIGHT;
//     bool collides = false;
//     for (int t = topTileY; t <= botTileY; t++) {
//       uint idx = t * map_width + tileX;
//       if (map[idx] > 0) {
//         collides = true;
//         break;
//       }
//     }
//     if (!collides) {
//       res.xdist = xvel;
//     }
//   }

//   if (yvel != 0) {
//     int tileY = (yedge + yvel) / TILE_HEIGHT;
//     int leftTileX = x / TILE_WIDTH;
//     int rightTileX = (x + width - 1) / TILE_WIDTH;
//     int dist = yvel;
//     for (int t = leftTileX; t <= rightTileX; t++) {
//       uint idx = tileY * map_width + t;
//       if (map[idx] > 0) {
//         int tile_realY = tileY * TILE_HEIGHT;

//         // entity would go through the ceiling
//         if (y >= (tile_realY + TILE_HEIGHT)) {
//           dist = (tile_realY + TILE_HEIGHT) - y;
//           res.hit_top = true;
//         }
//         // entity would go through the floor
//         else {
//           dist = tile_realY - (y + height);
//           res.hit_floor = true;
//         }

//         break;
//       }
//     }

//     res.ydist = dist;
//   }

//   return res;
// }

// void game_updateFrame() {
//   OAM[0] = oam_object_backbuffer[0];

//   Vec_u16 cam = {0};
//   Vec_u16 cam_player_offset = {0};
//   camera_for_player(&player, &cam, &cam_player_offset);

//   REG_BG0HOFS = cam.x;
//   REG_BG0VOFS = cam.y;

//   REG_BG1HOFS = cam.x;
//   REG_BG1VOFS = cam.y;

//   REG_BG2HOFS = cam.x;
//   REG_BG2VOFS = cam.y;

//   // bg_scroll_x = 0;
//   // bg_scroll_y = 0;

//   // player_draw(&player, &camera);

//   // for(uint i = 0; i < MAX_NPC_COUNT; i++) {
//   //     if (enemies[i].isInit) {
//   //         enemies[i].draw(&enemies[i], &camera, enem_sprites);
//   //     }
//   // }
// }

StateType GameState::update() {

  level.update(counter);
  player.update(counter);

  ++counter;

  // static uint delay = 0;
  // delay = (delay + 1) % 12;
  // // spawnEnemy();

  // static bool isJumping;
  // static int yvel;

  // if (keys & KEY_START) {
  //   return MENU;
  // }

  // int xB = 0;
  // int xvel = 0;
  // if (keys & KEY_RIGHT) {
  //   xB = player.hitbox.x + player.hitbox.width - 1;
  //   xvel = RUN_VEL;
  //   player.facingLeft = false;
  // } else if (keys & KEY_LEFT) {
  //   xB = player.hitbox.x;
  //   xvel = -RUN_VEL;
  //   player.facingLeft = true;
  // }

  // int yB = 0;
  // if (keys & KEY_A && !isJumping) {
  //   yvel = JUMP_VEL;
  //   isJumping = true;
  // } else {
  //   yvel = min(yvel + 1, GRAVITY);
  // }

  // if (yvel <= 0) {
  //   yB = player.hitbox.y;
  // } else {
  //   yB = player.hitbox.y + player.hitbox.height - 1;
  // }

  // gravdata dat =
  //     calc_collide(xvel, xB, player.hitbox.x, player.hitbox.width, yvel, yB,
  //                  player.hitbox.y, player.hitbox.height,
  //                  active_level->layer1, active_level->width,
  //                  active_level->height);

  // if (isJumping) {
  //   if (dat.hit_floor) {
  //     isJumping = false;
  //   }
  //   if (dat.hit_top) {
  //     yvel = 0;
  //   }
  // } else {
  //   if (dat.ydist != 0) {
  //     isJumping = true;
  //   }
  // }

  // int new_x = player.position.x + dat.xdist;
  // int new_y = player.position.y + dat.ydist;

  // if (new_x < 0) {
  //   new_x = 0;
  // }

  // if (new_x > max_move_x) {
  //   new_x = max_move_x;
  // }

  // if (new_y < 0) {
  //   new_y = 0;
  // }

  // if (new_y > max_move_y) {
  //   new_y = max_move_y;
  // }

  // player_setPosition(&player, new_x, new_y);

  // player.isMoving = (xvel != 0) || (dat.ydist != 0);
  // player.isJumping = isJumping;

  // player_tick(&player, delay);

  // // for(uint i = 0; i < MAX_NPC_COUNT; i++) {
  // //     if (enemies[i].isInit) {
  // //         enemies[i].tick(&enemies[i], calc_collide, delay);
  // //     }
  // // }

  // game_updateFrame();

  return GAME;
}
