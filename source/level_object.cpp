#include <gba.h>

#include "level_object.h"
#include "util.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

/*!
 * Get the screen entry index for a tile-coord pair.
 * This is the fast (and possibly unsafe) way.
 * Lifted from:
 * https://www.coranac.com/tonc/text/regbg.htm
 *  \param bgcnt    Control flags for this background (to find its size)
 */
uint se_index_fast(uint tx, uint ty, u16 bgcnt) {
  uint n = tx + ty * 32;
  if (tx >= 32)
    n += 0x03E0; // Size of one screenblock (in tiles) - Size of one row of
                 // tiles (1024-32)
  if (ty >= 32 && (bgcnt & BG_SIZE_3) == BG_SIZE_3)
    n += 0x0400; // Size of one screenblock (1024)
  return n;
}

/*!
 * Paints a single layer of 16x16 tiles to memory.
 * \param base_sb Base screenblock entry to start painting at.
 * \param bgcnt Background control register you're painting to.
 * \param tileset Array containing the reduced tiles to paint.
 * \param tiles The actual map data that references the tileset.
 * \param height The height of the map (in 16x16 tiles)
 * \param width The width of the map (in 16x16 tiles)
 */
void LevelObject::load_tiles(u16 *base_sb, u16 bgcnt, const u16 *tileset,
                             const u16 *tiles, int height, int width) {
  for (uint y = 0; y < height; y++) {
    for (uint x = 0; x < width; x++) {
      uint screenblock = se_index_fast(x * 2, y * 2, bgcnt);
      u16 *dest = base_sb + screenblock;

      u16 tile = tiles[y * width + x];
      u16 base_idx = tile * 4;
      for (uint i = 0; i < 2; i++) {
        u16 o1 = tileset[base_idx + i];
        u16 o2 = tileset[base_idx + i + 2];
        dest[i] = o1;
        dest[width + i] = o2;
      }
    }
  }
}

void LevelObject::load_tile_palette(const u16 *pal, const int pal_len,
                                    const uint *tiles, const int tiles_len) {
  // TODO: specify char base block

  /* load the palette from the image into palette memory via dma */
  dmaCopy(pal, BG_PALETTE, pal_len);
  dmaCopy(tiles, CHAR_BASE_BLOCK(0), tiles_len);
}

void LevelObject::init_backgrounds() {
  // TODO: specify char and screen blocks

  /* set all control the bits in this register */
  REG_BG0CNT =
      2 |                /* priority, 0 is highest, 3 is lowest */
      (CHAR_BASE(0)) |   /* the char block the image data is stored in */
      (BG_MOSAIC) |      /* the mosaic flag */
      (BG_256_COLOR) |   /* color mode, 0 is 16 colors, 1 is 256 colors */
      (SCREEN_BASE(4)) | /* the screen block the tile data is stored in */
      (BG_WRAP) |        /* wrapping flag */
      (BG_SIZE_3);       /* bg size 3 is 512x512 */

  REG_BG1CNT =
      1 |                /* priority, 0 is highest, 3 is lowest */
      (CHAR_BASE(0)) |   /* the char block the image data is stored in */
      (BG_MOSAIC) |      /* the mosaic flag */
      (BG_256_COLOR) |   /* color mode, 0 is 16 colors, 1 is 256 colors */
      (SCREEN_BASE(8)) | /* the screen block the tile data is stored in */
      (BG_WRAP) |        /* wrapping flag */
      (BG_SIZE_3);       /* bg size 3 is 512x512 */

  REG_BG2CNT =
      0 |                 /* priority, 0 is highest, 3 is lowest */
      (CHAR_BASE(0)) |    /* the char block the image data is stored in */
      (BG_MOSAIC) |       /* the mosaic flag */
      (BG_256_COLOR) |    /* color mode, 0 is 16 colors, 1 is 256 colors */
      (SCREEN_BASE(12)) | /* the screen block the tile data is stored in */
      (BG_WRAP) |         /* wrapping flag */
      (BG_SIZE_3);        /* bg size 3 is 512x512 */

  // Set the right display mode - all three backgrounds
  REG_DISPCNT = MODE_0 | BG0_ON | BG1_ON | BG2_ON | OBJ_ON | OBJ_1D_MAP;
}

void LevelObject::load(const u16 *pal, const int pal_len, const uint *tiles,
                       const int tiles_len, int height, int width,
                       const u16 *meta_tiles, const u16 *bg0, const u16 *bg1,
                       const u16 *bg2) {

  init_backgrounds();

  size_tiles = Rectangle(0, 0, width, height);
  size_px = Rectangle(0, 0, width * TILE_WIDTH, height * TILE_HEIGHT);
  viewport = Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  load_tile_palette(pal, pal_len, tiles, tiles_len);

  this->bg0 = bg0;
  this->bg1 = bg1;
  this->bg2 = bg2;

  load_tiles((u16 *)SCREEN_BASE_BLOCK(4), REG_BG0CNT, meta_tiles, bg0, height,
             width);
  load_tiles((u16 *)SCREEN_BASE_BLOCK(8), REG_BG1CNT, meta_tiles, bg1, height,
             width);
  load_tiles((u16 *)SCREEN_BASE_BLOCK(12), REG_BG1CNT, meta_tiles, bg2, height,
             width);

  _initialized = true;
}

void LevelObject::update(PlayerInputComponent &input) {}

void LevelObject::move_viewport(int d_x, int d_y) {
  int new_x = viewport.x() + d_x;
  int new_y = viewport.y() + d_y;

  if (new_x < 0) {
    _at_left_edge = true;
    new_x = 0;
  } else {
    _at_left_edge = false;
  }

  if (new_x + viewport.width() > size_px.width()) {
    _at_right_edge = true;
    new_x = size_px.width() - viewport.width();
  } else {
    _at_right_edge = false;
  }

  if (new_y < 0) {
    _at_top_edge = true;
    new_y = 0;
  } else {
    _at_top_edge = false;
  }

  if (new_y + viewport.height() > size_px.height()) {
    _at_bottom_edge = true;
    new_y = size_px.height() - viewport.height();
  } else {
    _at_bottom_edge = false;
  }

  int diff_x = new_x - viewport.x();
  int diff_y = new_y - viewport.y();

  viewport.set_x(new_x);
  viewport.set_y(new_y);

  if (diff_x < 0) {
    auto val = (u16)(0 - diff_x);
    bg0_hscroll -= val;
    bg1_hscroll -= val;
    bg2_hscroll -= val;
  } else if (diff_x > 0) {
    bg0_hscroll += diff_x;
    bg1_hscroll += diff_x;
    bg2_hscroll += diff_x;
  }

  if (diff_y < 0) {
    auto val = (u16)(0 - diff_y);
    bg0_vscroll -= val;
    bg1_vscroll -= val;
    bg2_vscroll -= val;
  } else if (diff_y > 0) {
    bg0_vscroll += diff_y;
    bg1_vscroll += diff_y;
    bg2_vscroll += diff_y;
  }

  REG_BG0HOFS = bg0_hscroll;
  REG_BG0VOFS = bg0_vscroll;
  REG_BG1HOFS = bg1_hscroll;
  REG_BG1VOFS = bg1_vscroll;
  REG_BG2HOFS = bg2_hscroll;
  REG_BG2VOFS = bg2_vscroll;
}

ResolvedMovement LevelObject::resolve_collision(Rectangle &hitbox, int xvel,
                                                int yvel) {
  ResolvedMovement res;

  int xedge = 0;
  if (xvel > 0) {
    xedge = hitbox.x() + hitbox.width() - 1;
  } else if (xvel < 0) {
    xedge = hitbox.x();
  }

  // maximum distance you can travel in the x axis before collision
  int xdist = xvel;

  int tileX = (xedge + xvel) / TILE_WIDTH;
  int topTileY = hitbox.y() / TILE_HEIGHT;
  int botTileY = (hitbox.y() + hitbox.height() - 1) / TILE_HEIGHT;
  // bool collides = false;
  for (int t = topTileY; t <= botTileY; t++) {
    uint idx = t * size_tiles.width() + tileX;
    if (bg1[idx] > 0) {
      int t_px = t * TILE_WIDTH;
      if (xvel < 0) {
        xdist = t_px + TILE_WIDTH - xedge;
      } else if (xvel > 0) {
        xdist = t_px - xedge;
      }
      break;
    }
  }

  res.x_dist = xdist;

  int yedge = 0;
  if (yvel > 0) {
    yedge = hitbox.y() + hitbox.height() - 1;
  } else if (yvel < 0) {
    yedge = hitbox.y();
  }

  // maximum distance you can go in the y axis before collision
  int ydist = yvel;

  int tileY = (yedge + yvel) / TILE_HEIGHT;
  int leftTileX = hitbox.x() / TILE_WIDTH;
  int rightTileX = (hitbox.x() + hitbox.width() - 1) / TILE_WIDTH;
  for (int t = leftTileX; t <= rightTileX; t++) {
    uint idx = tileY * size_tiles.width() + t;
    if (bg1[idx] > 0) {
      int t_px = idx / size_tiles.width() * TILE_HEIGHT;

      if (yvel < 0) {
        ydist = t_px + TILE_HEIGHT - yedge;
      } else if (yvel > 0) {
        ydist = t_px - yedge;
      }

      break;
    }
  }

  res.y_dist = ydist;

  return res;
}
