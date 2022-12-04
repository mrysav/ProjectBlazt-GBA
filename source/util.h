#pragma once

#include <stdbool.h>
#include <stdint.h>

// Makes life easier. If you're like me and have trouble reading
// excessive defines... my bad.
#define uint unsigned int

/*!
 * Get the screen entry index for a tile-coord pair.
 * This is the fast (and possibly unsafe) way.
 * Lifted from:
 * https://www.coranac.com/tonc/text/regbg.htm
 *  \param bgcnt    Control flags for this background (to find its size)
 */
uint se_index_fast(uint tx, uint ty, u16 bgcnt);

/*!
 * Paints a single layer of 16x16 tiles to memory.
 * \param base_sb Base screenblock entry to start painting at.
 * \param bgcnt Background control register you're painting to.
 * \param tileset Array containing the reduced tiles to paint.
 * \param tiles The actual map data that references the tileset.
 * \param height The height of the map (in 16x16 tiles)
 * \param width The width of the map (in 16x16 tiles)
 */
void load_tiles(u16 *base_sb, u16 bgcnt, const u16 *tileset, const u16 *tiles,
                int height, int width);
