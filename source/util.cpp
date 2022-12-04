#include <gba.h>

#include "util.h"

uint se_index_fast(uint tx, uint ty, u16 bgcnt) {
  uint n = tx + ty * 32;
  if (tx >= 32)
    n += 0x03E0; // Size of one screenblock (in tiles) - Size of one row of
                 // tiles (1024-32)
  if (ty >= 32 && (bgcnt & BG_SIZE_3) == BG_SIZE_3)
    n += 0x0400; // Size of one screenblock (1024)
  return n;
}

void load_tiles(u16 *base_sb, u16 bgcnt, const u16 *tileset, const u16 *tiles,
                int height, int width) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint screenblock = se_index_fast(x * 2, y * 2, bgcnt);
      u16 *dest = base_sb + screenblock;

      u16 tile = tiles[y * width + x];
      u16 base_idx = tile * 4;
      for (uint i = 0; i < 2; i++) {
        u16 o1 = tileset[base_idx + i];
        u16 o2 = tileset[base_idx + i + 2];
        dest[i] = o1;
        // width of a screenblock
        dest[32 + i] = o2;
      }
    }
  }
}
