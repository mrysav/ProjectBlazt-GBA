#pragma once

#include <stdbool.h>
#include <stdint.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct Vec {
    int_fast32_t x;
    int_fast32_t y;
} Vec;

typedef struct Rect {
    int_fast32_t x;
    int_fast32_t y;
    int_fast32_t width;
    int_fast32_t height;
} Rect;

bool intersects(Rect* rect1, Rect* rect2);
