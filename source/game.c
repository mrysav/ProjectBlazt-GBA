#include "game.h"

#include <gba.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Main tile palette
#include "im_bg_basic.h"

// Maps
#include "level_1.h"
#include "map_mario_1.h"

// Utilities
#include "display.h"
#include "player.h"
#include "util.h"

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

Player player = {0};
// NPC enemies[MAX_NPC_COUNT] = {0};

// Set when loadResources() is called
uint map_total_width;
uint map_total_height;
uint max_move_x;
uint max_move_y;
uint max_camera_x;
uint max_camera_y;

// Rect camera = {-CENTER_PLAYER_SCREEN_X, -CENTER_PLAYER_SCREEN_Y, SCREEN_WIDTH, SCREEN_HEIGHT};

int numEnem = 0;

OBJATTR oam_object_backbuffer[128];

typedef struct level_data
{
  uint height;
  uint width;
  uint len;

  const uint16_t *layer0;
  const uint16_t *layer1;
  const uint16_t *layer2;

} level_data;

level_data const MARIO_1_1 = {
    MAP_MARIO_1_HEIGHT,
    MAP_MARIO_1_WIDTH,
    MAP_MARIO_1_LEN,
    MAP_MARIO_1_BG0,
    MAP_MARIO_1_BG1,
    MAP_MARIO_1_BG2,
};

level_data const LEVEL_1_DBG = {
    LEVEL_1_HEIGHT,
    LEVEL_1_WIDTH,
    LEVEL_1_LEN,
    LEVEL_1_BG0,
    LEVEL_1_BG1,
    LEVEL_1_BG2,
};

level_data const *active_level;

void loadPalette()
{
  /* load the palette from the image into palette memory via dma */
  dmaCopy(im_bg_basicPal, BG_PALETTE, im_bg_basicPalLen);
  dmaCopy(im_bg_basicTiles, CHAR_BASE_BLOCK(0), im_bg_basicTilesLen);

  /* set all control the bits in this register */
  REG_BG0CNT = 2 |                /* priority, 0 is highest, 3 is lowest */
               (CHAR_BASE(0)) |   /* the char block the image data is stored in */
               (BG_MOSAIC) |      /* the mosaic flag */
               (BG_256_COLOR) |   /* color mode, 0 is 16 colors, 1 is 256 colors */
               (SCREEN_BASE(8)) | /* the screen block the tile data is stored in */
               (BG_WRAP) |        /* wrapping flag */
               (BG_SIZE_3);       /* bg size 3 is 512x512 */

  REG_BG1CNT = 1 |                 /* priority, 0 is highest, 3 is lowest */
               (CHAR_BASE(0)) |    /* the char block the image data is stored in */
               (BG_MOSAIC) |       /* the mosaic flag */
               (BG_256_COLOR) |    /* color mode, 0 is 16 colors, 1 is 256 colors */
               (SCREEN_BASE(12)) | /* the screen block the tile data is stored in */
               (BG_WRAP) |         /* wrapping flag */
               (BG_SIZE_3);        /* bg size 3 is 512x512 */

  REG_BG2CNT = 0 |                 /* priority, 0 is highest, 3 is lowest */
               (CHAR_BASE(0)) |    /* the char block the image data is stored in */
               (BG_MOSAIC) |       /* the mosaic flag */
               (BG_256_COLOR) |    /* color mode, 0 is 16 colors, 1 is 256 colors */
               (SCREEN_BASE(16)) | /* the screen block the tile data is stored in */
               (BG_WRAP) |         /* wrapping flag */
               (BG_SIZE_3);        /* bg size 3 is 512x512 */
}

/*!
 * Get the screen entry index for a tile-coord pair.
 * This is the fast (and possibly unsafe) way.
 * Lifted from:
 * https://www.coranac.com/tonc/text/regbg.htm
 *  \param bgcnt    Control flags for this background (to find its size)
 */
uint se_index_fast(uint tx, uint ty, u16 bgcnt)
{
  uint n = tx + ty * 32;
  if (tx >= 32)
    n += 0x03E0; // Size of one screenblock (in tiles) - Size of one row of tiles (1024-32)
  if (ty >= 32 && (bgcnt & BG_SIZE_3) == BG_SIZE_3)
    n += 0x0400; // Size of one screenblock (1024)
  return n;
}

/*!
 * Paints a single layer of 16x16 tiles to memory.
 * \param base_sb Base screenblock entry to start painting at.
 * \param bgcnt Background control register of the background you're painting to.
 * \param tileset Array containing the reduced tiles to paint.
 * \param tiles The actual map data that references the tileset.
 * \param height The height of the map (in 16x16 tiles)
 * \param width The width of the map (in 16x16 tiles)
 */
void load_layer(u16 *base_sb, u16 bgcnt, const u16 *tileset, const u16 *tiles, uint height, uint width)
{
  for (uint y = 0; y < height; y++)
  {
    for (uint x = 0; x < width; x++)
    {
      uint screenblock = se_index_fast(x * 2, y * 2, bgcnt);
      u16 *dest = base_sb + screenblock;

      u16 tile = tiles[y * width + x];
      u16 base_idx = tile * 4;
      for (uint i = 0; i < 2; i++)
      {
        u16 o1 = tileset[base_idx + i];
        u16 o2 = tileset[base_idx + i + 2];
        dest[i] = o1;
        dest[width + i] = o2;
      }
    }
  }
}

void clear_sprites()
{
  for (int i = 0; i < 128; i++)
  {
    OAM[i].attr0 = SCREEN_HEIGHT;
    OAM[i].attr1 = SCREEN_WIDTH;
  }
}

void game_loadResources()
{
  // Set the right display mode
  REG_DISPCNT = MODE_0 | BG0_ON | BG1_ON | BG2_ON | OBJ_ON | OBJ_1D_MAP;

  active_level = &LEVEL_1_DBG;

  loadPalette();

  load_layer(SCREEN_BASE_BLOCK(8), REG_BG0CNT, im_bg_basicMetaTiles,
             active_level->layer0, active_level->height, active_level->width);

  load_layer(SCREEN_BASE_BLOCK(12), REG_BG1CNT, im_bg_basicMetaTiles,
             active_level->layer1, active_level->height, active_level->width);

  player_loadSprites();

  clear_sprites();

  player_init(&player, &oam_object_backbuffer[0]);

  map_total_height = active_level->height * TILE_HEIGHT;
  map_total_width = active_level->width * TILE_WIDTH;

  max_move_x = map_total_width - player.hitbox.width;
  max_move_y = map_total_height - player.hitbox.height;

  max_camera_x = map_total_width - SCREEN_WIDTH;
  max_camera_y = map_total_height - SCREEN_HEIGHT;
}

void game_unloadResources()
{
  player_destroy(&player);
  clear_sprites();
}

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

void camera_for_player(Player *player, Vec_u16 *camera, Vec_u16 *offset)
{

  // The "happy case" - if we can center the camera on the player, we're good
  u16 centered_x = player->position.x - CENTER_PLAYER_SCREEN_X;
  u16 centered_y = player->position.y - CENTER_PLAYER_SCREEN_Y;
  offset->x = 0;
  offset->y = 0;
  camera->x = centered_x;
  camera->x = centered_y;

  // Underflow
  if (centered_x > player->position.x)
  {
    centered_x = 0;
  }
}

gravdata calc_collide(int xvel, int xedge, int x, int width,
                      int yvel, int yedge, int y, int height,
                      const u16 *map, u16 map_width, u16 map_height)
{
  gravdata res = {0};

  if (xvel != 0)
  {
    int tileX = (xedge + xvel) / TILE_WIDTH;
    int topTileY = y / TILE_HEIGHT;
    int botTileY = (y + height - 1) / TILE_HEIGHT;
    bool collides = false;
    for (int t = topTileY; t <= botTileY; t++)
    {
      uint idx = t * map_width + tileX;
      if (map[idx] > 0)
      {
        collides = true;
        break;
      }
    }
    if (!collides)
    {
      res.xdist = xvel;
    }
  }

  if (yvel != 0)
  {
    int tileY = (yedge + yvel) / TILE_HEIGHT;
    int leftTileX = x / TILE_WIDTH;
    int rightTileX = (x + width - 1) / TILE_WIDTH;
    int dist = yvel;
    for (int t = leftTileX; t <= rightTileX; t++)
    {
      uint idx = tileY * map_width + t;
      if (map[idx] > 0)
      {
        int tile_realY = tileY * TILE_HEIGHT;

        // entity would go through the ceiling
        if (y >= (tile_realY + TILE_HEIGHT))
        {
          dist = (tile_realY + TILE_HEIGHT) - y;
          res.hit_top = true;
        }
        // entity would go through the floor
        else
        {
          dist = tile_realY - (y + height);
          res.hit_floor = true;
        }

        break;
      }
    }

    res.ydist = dist;
  }

  return res;
}

State game_processInput(uint16_t keys)
{
  static uint delay = 0;
  delay = (delay + 1) % 12;

  //spawnEnemy();

  static bool isJumping;
  static int yvel;

  if (keys & KEY_START)
  {
    return MENU;
  }

  int xB = 0;
  int xvel = 0;
  if (keys & KEY_RIGHT)
  {
    xB = player.hitbox.x + player.hitbox.width - 1;
    xvel = RUN_VEL;
    player.facingLeft = false;
  }
  else if (keys & KEY_LEFT)
  {
    xB = player.hitbox.x;
    xvel = -RUN_VEL;
    player.facingLeft = true;
  }

  int yB = 0;
  if (keys & KEY_A && !isJumping)
  {
    yvel = JUMP_VEL;
    isJumping = true;
  }
  else
  {
    yvel = min(yvel + 1, GRAVITY);
  }

  if (yvel <= 0)
  {
    yB = player.hitbox.y;
  }
  else
  {
    yB = player.hitbox.y + player.hitbox.height - 1;
  }

  gravdata dat = calc_collide(xvel, xB, player.hitbox.x, player.hitbox.width,
                              yvel, yB, player.hitbox.y, player.hitbox.height,
                              active_level->layer1, active_level->width, active_level->height);

  if (isJumping)
  {
    if (dat.hit_floor)
    {
      isJumping = false;
    }
    if (dat.hit_top)
    {
      yvel = 0;
    }
  }
  else
  {
    if (dat.ydist != 0)
    {
      isJumping = true;
    }
  }

  int new_x = player.position.x + dat.xdist;
  int new_y = player.position.y + dat.ydist;

  if (new_x < 0)
  {
    new_x = 0;
  }

  if (new_x > max_move_x)
  {
    new_x = max_move_x;
  }

  if (new_y < 0)
  {
    new_y = 0;
  }

  if (new_y > max_move_y)
  {
    new_y = max_move_y;
  }

  player_setPosition(&player, new_x, new_y);

  player.isMoving = (xvel != 0) || (dat.ydist != 0);
  player.isJumping = isJumping;

  player_tick(&player, delay);

  // for(uint i = 0; i < MAX_NPC_COUNT; i++) {
  //     if (enemies[i].isInit) {
  //         enemies[i].tick(&enemies[i], calc_collide, delay);
  //     }
  // }

  return GAME;
}

void game_updateFrame()
{
  OAM[0] = oam_object_backbuffer[0];

  Vec_u16 cam = {0};
  Vec_u16 cam_player_offset = {0};
  camera_for_player(&player, &cam, &cam_player_offset);

  REG_BG0HOFS = cam.x;
  REG_BG0VOFS = cam.y;

  REG_BG1HOFS = cam.x;
  REG_BG1VOFS = cam.y;

  REG_BG2HOFS = cam.x;
  REG_BG2VOFS = cam.y;

  //bg_scroll_x = 0;
  //bg_scroll_y = 0;

  // player_draw(&player, &camera);

  // for(uint i = 0; i < MAX_NPC_COUNT; i++) {
  //     if (enemies[i].isInit) {
  //         enemies[i].draw(&enemies[i], &camera, enem_sprites);
  //     }
  // }
}

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources};
