#include <gba.h>

#include "hud.h"
#include "level_object.h"
#include "util.h"

#include "pullrequest_object.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

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
  REG_BG1CNT =
      2 |                /* priority, 0 is highest, 3 is lowest */
      (CHAR_BASE(0)) |   /* the char block the image data is stored in */
      (BG_MOSAIC) |      /* the mosaic flag */
      (BG_256_COLOR) |   /* color mode, 0 is 16 colors, 1 is 256 colors */
      (SCREEN_BASE(4)) | /* the screen block the tile data is stored in */
      (BG_WRAP) |        /* wrapping flag */
      (BG_SIZE_3);       /* bg size 3 is 512x512 */

  REG_BG2CNT =
      1 |                /* priority, 0 is highest, 3 is lowest */
      (CHAR_BASE(0)) |   /* the char block the image data is stored in */
      (BG_MOSAIC) |      /* the mosaic flag */
      (BG_256_COLOR) |   /* color mode, 0 is 16 colors, 1 is 256 colors */
      (SCREEN_BASE(8)) | /* the screen block the tile data is stored in */
      // (BG_WRAP) |        /* wrapping flag */
      (BG_SIZE_3); /* bg size 3 is 512x512 */

  // Set the right display mode - all three backgrounds are regular backgrounds
  REG_DISPCNT = MODE_0 | BG0_ON | BG1_ON | BG2_ON | OBJ_ON | OBJ_1D_MAP;
}

void LevelObject::load(const u16 *pal, const int pal_len, const uint *tiles,
                       const int tiles_len, int height, int width,
                       const u16 *meta_tiles, const u16 *bg0, const u16 *bg1,
                       PullRequestObject *prs, int pr_count) {

  init_backgrounds();

  size_tiles = Rectangle(0, 0, width, height);
  size_px = Rectangle(0, 0, width * TILE_WIDTH, height * TILE_HEIGHT);
  viewport = Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  load_tile_palette(pal, pal_len, tiles, tiles_len);

  this->bg0 = bg0;
  this->bg1 = bg1;

  load_tiles((u16 *)SCREEN_BASE_BLOCK(4), REG_BG1CNT, meta_tiles, bg0, height,
             width);
  load_tiles((u16 *)SCREEN_BASE_BLOCK(8), REG_BG2CNT, meta_tiles, bg1, height,
             width);

  hud.initialize();
  hud.update(30, 1);

  this->pullrequests = prs;
  this->pr_count = pr_count;

  _initialized = true;
}

void LevelObject::update(u16 &counter, PlayerInputComponent &input) {
  static u16 lastUpdate = 0;
  uint timerVal = hud.timer();
  // running at about 60 fps
  if (counter - lastUpdate > 60) {
    timerVal--;
    lastUpdate = counter;
  }
  hud.update(timerVal, 0);
}

void LevelObject::move_viewport(int d_x, int d_y) {
  int new_x = viewport.x() + d_x;
  int new_y = viewport.y() + d_y;

  // if (new_x < 0) {
  //   _at_left_edge = true;
  //   new_x = 0;
  // } else {
  //   _at_left_edge = false;
  // }

  // if (new_x + viewport.width() > size_px.width()) {
  //   _at_right_edge = true;
  //   new_x = size_px.width() - viewport.width();
  // } else {
  //   _at_right_edge = false;
  // }

  // if (new_y < 0) {
  //   _at_top_edge = true;
  //   new_y = 0;
  // } else {
  //   _at_top_edge = false;
  // }

  // if (new_y + viewport.height() > size_px.height()) {
  //   _at_bottom_edge = true;
  //   new_y = size_px.height() - viewport.height();
  // } else {
  //   _at_bottom_edge = false;
  // }

  int diff_x = new_x - viewport.x();
  int diff_y = new_y - viewport.y();

  viewport.set_x(new_x);
  viewport.set_y(new_y);

  if (diff_x < 0) {
    auto val = (u16)(0 - diff_x);
    bg0_hscroll -= val;
    bg1_hscroll -= val;
  } else if (diff_x > 0) {
    bg0_hscroll += diff_x;
    bg1_hscroll += diff_x;
  }

  if (diff_y < 0) {
    auto val = (u16)(0 - diff_y);
    bg0_vscroll -= val;
    bg1_vscroll -= val;
  } else if (diff_y > 0) {
    bg0_vscroll += diff_y;
    bg1_vscroll += diff_y;
  }

  for (int i = 0; i < pr_count; i++) {
    pullrequests[i].move(-diff_x, -diff_y, viewport);
  }

  REG_BG1HOFS = bg0_hscroll;
  REG_BG1VOFS = bg0_vscroll;
  REG_BG2HOFS = bg1_hscroll;
  REG_BG2VOFS = bg1_vscroll;
}

ResolvedMovement LevelObject::resolve_collision(Rectangle &hitbox, int xvel,
                                                int yvel) {
  ResolvedMovement res;

  if (xvel != 0) {

    int xedge = 0;
    if (xvel > 0) {
      xedge = hitbox.x() + hitbox.width() - 1;
    } else if (xvel < 0) {
      xedge = hitbox.x();
    }

    bool x_collide = false;
    int tileX = (xedge + xvel) / TILE_WIDTH;
    int topTileY = hitbox.y() / TILE_HEIGHT;
    int botTileY = (hitbox.y() + hitbox.height()) % TILE_HEIGHT > 0
                       ? topTileY + 1
                       : topTileY;
    for (int t = topTileY; t <= botTileY; t++) {
      uint idx = t * size_tiles.width() + tileX;
      if (bg1[idx] > 0) {
        x_collide = true;
        break;
      }
    }

    // // maximum distance you can travel in the x axis before collision
    // int xdist = xvel;

    if (!x_collide) {
      res.x_dist = xvel;
    } // else {
    //   int t_px = tileX * TILE_WIDTH;
    //   if (xvel < 0) {
    //     res.x_dist = t_px + TILE_WIDTH - xedge;
    //   } else if (xvel > 0) {
    //     res.x_dist = t_px - xedge;
    //   }
    // }
  }

  if (yvel != 0) {
    int yedge = 0;
    if (yvel > 0) {
      yedge = hitbox.y() + hitbox.height();
    } else if (yvel < 0) {
      yedge = hitbox.y();
    }

    // maximum distance you can go in the y axis before collision
    int ydist = yvel;

    int tileY = (yedge + yvel) / TILE_HEIGHT;
    int leftTileX = hitbox.x() / TILE_WIDTH;
    int rightTileX = (hitbox.x() + hitbox.width()) / TILE_WIDTH;
    for (int t = leftTileX; t <= rightTileX; t++) {
      uint idx = tileY * size_tiles.width() + t;
      if (bg1[idx] > 0) {
        int t_px = tileY * TILE_HEIGHT;

        if (yvel < 0) {
          ydist = t_px + TILE_HEIGHT - yedge;
        } else if (yvel > 0) {
          ydist = t_px - yedge;
        }

        break;
      }
    }

    res.y_dist = ydist;
  }

  return res;
}
