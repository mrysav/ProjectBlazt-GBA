#pragma once

#include "player_input_component.h"
#include "position_component.h"
#include "rectangle.h"
#include "util.h"

class ResolvedMovement {
public:
  int x_dist = 0;
  int y_dist = 0;
};

class LevelObject {
public:
  LevelObject(){};

  void load(const u16 *pal, const int pal_len, const uint *tiles,
            const int tiles_len, int height, int width, const u16 *meta_tiles,
            const u16 *bg0, const u16 *bg1, const u16 *bg2);

  void update(PlayerInputComponent &input);

  void move_viewport(int d_x, int d_y);

  ResolvedMovement resolve_collision(Rectangle &hitbox, int xvel, int yvel);

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

  bool _initialized = false;

  bool _at_right_edge = false;
  bool _at_left_edge = false;
  bool _at_top_edge = false;
  bool _at_bottom_edge = false;

  Rectangle size_tiles;
  Rectangle size_px;
  Rectangle viewport;

  const u16 *bg0;
  const u16 *bg1;
  const u16 *bg2;

  u16 bg0_vscroll = 0;
  u16 bg0_hscroll = 0;
  u16 bg1_vscroll = 0;
  u16 bg1_hscroll = 0;
  u16 bg2_vscroll = 0;
  u16 bg2_hscroll = 0;
};
