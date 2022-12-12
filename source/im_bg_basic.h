
//{{BLOCK(im_bg_basic)

//======================================================================
//
//	im_bg_basic, 256x256@8, 
//	Transparent color : FF,00,E1
//	+ palette 256 entries, not compressed
//	+ 51 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 16x16 
//	Metatiled by 2x2
//	Total size: 512 + 3264 + 152 + 512 = 4440
//
//	Time-stamp: 2022-12-12, 08:56:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_IM_BG_BASIC_H
#define GRIT_IM_BG_BASIC_H

#define im_bg_basicTilesLen 3264
extern const unsigned int im_bg_basicTiles[816];

#define im_bg_basicMetaTilesLen 152
extern const unsigned short im_bg_basicMetaTiles[76];

#define im_bg_basicMetaMapLen 512
extern const unsigned short im_bg_basicMetaMap[256];

#define im_bg_basicPalLen 512
extern const unsigned short im_bg_basicPal[256];

#endif // GRIT_IM_BG_BASIC_H

//}}BLOCK(im_bg_basic)
