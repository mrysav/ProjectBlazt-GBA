#pragma once

#include <stdbool.h>
#include <stdint.h>

#define uint unsigned int

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct Vec {
    int_fast16_t x;
    int_fast16_t y;
} Vec;

typedef struct Rect {
    int_fast16_t x;
    int_fast16_t y;
    int_fast16_t width;
    int_fast16_t height;
} Rect;

bool intersects(Rect* rect1, Rect* rect2);

// TODO: grosssssss
typedef struct gravdata {
    bool hit_top;
    bool hit_floor;
    int_fast16_t xdist;
    int_fast16_t ydist;
} gravdata;

// TODO: ewwwwwwwww
typedef gravdata (*jumpfunc) (int_fast16_t xvel, int_fast16_t xedge, int_fast16_t x, int_fast16_t width, int_fast16_t yvel, int_fast16_t yedge, int_fast16_t y, int_fast16_t height);
