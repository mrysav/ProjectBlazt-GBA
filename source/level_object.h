#pragma once

#include "player_input_component.h"
#include "rectangle.h"
#include "util.h"

class LevelObject {
public:
  LevelObject(){};
  LevelObject(const u16 *pal, const int pal_len, const uint *tiles,
              const int tiles_len, int height, int width, const u16 *meta_tiles,
              const u16 *bg0, const u16 *bg1, const u16 *bg2);

  void update(u16 counter);

  void move_viewport(int d_x, int d_y);

  bool initialized() { return _initialized; }

  bool at_right_edge() { return _at_right_edge; }
  bool at_left_edge() { return _at_left_edge; }
  bool at_top_edge() { return _at_top_edge; }
  bool at_bottom_edge() { return _at_bottom_edge; }

private:
  void init_backgrounds();
  void load_tiles(u16 *base_sb, u16 bgcnt, const u16 *tileset, const u16 *tiles,
                  int height, int width);
  void load_tile_palette(const u16 *pal, const int pal_len, const uint *tiles,
                         const int tiles_len);

  PlayerInputComponent input = PlayerInputComponent();

  bool _initialized = false;

  bool _at_right_edge = false;
  bool _at_left_edge = false;
  bool _at_top_edge = false;
  bool _at_bottom_edge = false;

  Rectangle size_tiles;
  Rectangle size_px;
  Rectangle viewport;

  u16 *bg0;
  u16 *bg1;
  u16 *bg2;
};
