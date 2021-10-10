#pragma once

#include <stdbool.h>
#include <stdint.h>

// Makes life easier. If you're like me and have trouble reading
// excessive defines... my bad.
#define uint unsigned int

#define max(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a > _b ? _a : _b;                                                         \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    __typeof__(a) _a = (a);                                                    \
    __typeof__(b) _b = (b);                                                    \
    _a < _b ? _a : _b;                                                         \
  })

typedef struct Vec {
  int x;
  int y;
} Vec;

typedef struct Vec_u16 {
  u16 x;
  u16 y;
} Vec_u16;

typedef struct Vec16 {
  short int x;
  short int y;
} Vec16;

typedef struct Rect {
  int x;
  int y;
  int width;
  int height;
} Rect;

bool intersects(Rect *rect1, Rect *rect2);

// TODO: grosssssss
typedef struct gravdata {
  bool hit_top;
  bool hit_floor;
  int xdist;
  int ydist;
} gravdata;
