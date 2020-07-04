
//{{BLOCK(block_tiles_reduced)

//======================================================================
//
//	block_tiles_reduced, 256x256@8, 
//	Transparent color : FF,00,E1
//	+ palette 256 entries, not compressed
//	+ 13 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 16x16 
//	Metatiled by 2x2
//	Total size: 512 + 832 + 64 + 512 = 1920
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLOCK_TILES_REDUCED_H
#define GRIT_BLOCK_TILES_REDUCED_H

#define block_tiles_reducedTilesLen 832
extern const unsigned int block_tiles_reducedTiles[208];

#define block_tiles_reducedMetaTilesLen 64
extern const unsigned short block_tiles_reducedMetaTiles[32];

#define block_tiles_reducedMetaMapLen 512
extern const unsigned short block_tiles_reducedMetaMap[256];

#define block_tiles_reducedPalLen 512
extern const unsigned short block_tiles_reducedPal[256];

#endif // GRIT_BLOCK_TILES_REDUCED_H

//}}BLOCK(block_tiles_reduced)
