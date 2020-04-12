#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <gba.h>

#include "game.h"

// Main tile palette
#include "block_tiles.h"

// Level 1 map
#include "sky_map_1.h"

// Utilities
#include "background.h"
#include "util.h"
#include "display.h"

#include "player.h"

// #include "npc.h"
// #include "red_dude.h"

// #include "level.h"
// #include "underground_1.h"

// Always 256???
#define PALETTE_SIZE 256

#define TS_WIDTH 4
#define TS_HEIGHT 28
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
// only draw up to one level of tiles offscreen
#define MIN_DRAW -16
#define MAX_DRAW 16

// const Level* ALL_LEVELS[3] = { &UNDERGROUND_1 };
// const Level* currentLevel;

#define GRAVITY 6
#define RUN_VEL 3
#define JUMP_VEL -12
#define MAX_NPC_COUNT 50

Player player = {0};
// NPC enemies[MAX_NPC_COUNT] = {0};

// Set when loadResources() is called
uint_fast16_t map_total_width;
uint_fast16_t map_total_height;
uint_fast16_t max_draw_x;
uint_fast16_t max_draw_y;
uint_fast16_t max_move_x;
uint_fast16_t max_move_y;
int_fast16_t player_x_offset;
int_fast16_t player_y_offset;
Rect camera = {0};

int_fast16_t numEnem = 0;

OBJATTR oam_object_backbuffer[128];

/* return a pointer to one of the 4 character blocks (0-3) */
volatile unsigned short* char_block(unsigned long block) {
    /* they are each 16K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x4000));
}

/* return a pointer to one of the 32 screen blocks (0-31) */
volatile unsigned short* screen_block(unsigned long block) {
    /* they are each 2K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x800));
}

void loadPalette() {
    /* load the palette from the image into palette memory*/
    for (int i = 0; i < block_tilesPalLen; i++) {
        BG_PALETTE[i] = block_tilesPal[i];
    }

    /* load the image into char block 0 (16 bits at a time) */
    volatile unsigned short* dest = char_block(0);
    unsigned short* image = (unsigned short*) block_tilesTiles;
    for (int i = 0; i < block_tilesTilesLen; i++) {
        dest[i] = image[i];
    }

    /* set all control the bits in this register */
    REG_BG0CNT = 0        |  /* priority, 0 is highest, 3 is lowest */
        (CHAR_BASE(0))    |  /* the char block the image data is stored in */
        (BG_MOSAIC)       |  /* the mosaic flag */
        (BG_256_COLOR)    |  /* color mode, 0 is 16 colors, 1 is 256 colors */
        (SCREEN_BASE(16)) |  /* the screen block the tile data is stored in */
        (BG_WRAP)         |  /* wrapping flag */
        (BG_SIZE_1);         /* bg size, 0 is 256x256 */
}

void loadMap() {
    /* load the tile data into screen block 16 */
    volatile unsigned short* dest = screen_block(16);
    for (int i = 0; i < (sky_map_1_width * sky_map_1_height); i++) {
        dest[i] = sky_map_1[i];
    }
}

void game_loadResources()
{
    loadPalette();
    loadMap();

    player_init(&player, &oam_object_backbuffer[0]);

    max_draw_x = map_total_width + MAX_DRAW;
    max_draw_y = map_total_height + MAX_DRAW;

    max_move_x = map_total_width - player.hitbox.width;
    max_move_y = map_total_height - player.hitbox.height;

    // TODO: Better constants
    player_x_offset = -((SCREEN_WIDTH / 2) - (TILE_WIDTH / 2));
    player_y_offset = -((SCREEN_HEIGHT / 2) - (TILE_HEIGHT / 2));

    camera.x = player_x_offset;
    camera.y = player_y_offset;
    camera.width = SCREEN_WIDTH;
    camera.height = SCREEN_HEIGHT;
}

void game_unloadResources()
{
    player_destroy(&player);
}

// void spawnEnemy() {
//     static int_fast8_t delay = 0;
//     delay = (++delay) % 30;
//     if (delay > 0) {
//         return;
//     }

//     if (numEnem >= 10) {
//         return;
//     }
//     enemies[numEnem] = red_dude;
//     Vec32 pos = {0,0};
//     enemies[numEnem].init(&enemies[numEnem], pos);
//     numEnem++;
// }

gravdata calc_collide(int_fast16_t xvel, int_fast16_t xedge, int_fast16_t x, int_fast16_t width,
                      int_fast16_t yvel, int_fast16_t yedge, int_fast16_t y, int_fast16_t height) {

    gravdata res = {0};

    if (xvel != 0)
    {
        int_fast16_t tileX = (xedge + xvel) / TILE_WIDTH;
        int_fast16_t topTileY = y / TILE_HEIGHT;
        int_fast16_t botTileY = (y + height - 1) / TILE_HEIGHT;
        bool collides = false;
        for (int_fast16_t t = topTileY; t <= botTileY; t++)
        {
            if (false) // TODO if (currentLevel->tiles[t][tileX] > 0)
            {
                collides = true;
                break;
            }
        }
        if (!collides)
        {
            res.xdist = xvel;
        }
    }

    if (yvel != 0)
    {
        int_fast16_t tileY = (yedge + yvel) / TILE_HEIGHT;
        int_fast16_t leftTileX = x / TILE_WIDTH;
        int_fast16_t rightTileX = (x + width - 1) / TILE_WIDTH;
        int_fast16_t dist = yvel;
        for (int_fast16_t t = leftTileX; t <= rightTileX; t++)
        {
            if (false) // TODO if (currentLevel->tiles[tileY][t] > 0)
            {
                int_fast16_t tile_realY = tileY * TILE_HEIGHT;

                // entity would go through the ceiling
                if (y >= (tile_realY + TILE_HEIGHT))
                {
                    dist = (tile_realY + TILE_HEIGHT) - y;
                    res.hit_top = true;
                }
                // entity would go through the floor
                else
                {
                    dist = tile_realY - (y + height);
                    res.hit_floor = true;
                }

                break;
            }
        }

        res.ydist = dist;
    }

    return res;
}

State game_processInput(uint16_t keys)
{
    static int_fast16_t delay;
    delay = (++delay) % 12;

    //spawnEnemy();

    static bool isJumping;
    static int_fast16_t yvel;

    if (keys & KEY_START)
    {
        return MENU;
    }

    int_fast16_t xB = 0;
    int_fast16_t xvel = 0;
    if (keys & KEY_RIGHT)
    {
        xB = player.hitbox.x + player.hitbox.width - 1;
        xvel = RUN_VEL;
    }
    else if (keys & KEY_LEFT)
    {
        xB = player.hitbox.x;
        xvel = -RUN_VEL;
    }

    if (xvel < 0) {
        player.facingLeft = true;
    } else if (xvel > 0) {
        player.facingLeft = false;
    }

    int_fast16_t yB = 0;
    if (keys & KEY_UP && !isJumping)
    {
        yvel = JUMP_VEL;
        isJumping = true;
    }
    else
    {
        yvel = min(yvel + 1, GRAVITY);
    }

    if (yvel <= 0)
    {
        yB = player.hitbox.y;
    }
    else
    {
        yB = player.hitbox.y + player.hitbox.height - 1;
    }

    gravdata dat = calc_collide(xvel, xB, player.hitbox.x, player.hitbox.width,
                                yvel, yB, player.hitbox.y, player.hitbox.height);

    if (isJumping) {
        if (dat.hit_floor) {
            isJumping = false;
        }
        if (dat.hit_top) {
            yvel = 0;
        }
    } else {
        if (dat.ydist != 0) {
            isJumping = true;
        }
    }

    int_fast16_t new_x = player.position.x + dat.xdist;
    int_fast16_t new_y = player.position.y + dat.ydist;

    if (new_x < 0) {
        new_x = 0;
    }

    if (new_x > max_move_x) {
        new_x = max_move_x;
    }

    if (new_y < 0) {
        new_y = 0;
    }

    if (new_y > max_move_y) {
        new_y = max_move_y;
    }

    player_setPosition(&player, new_x, new_y);

    player.isMoving = (xvel != 0) || (dat.ydist != 0);
    player.isJumping = isJumping;

    player_tick(&player, delay);

    // for(uint_fast16_t i = 0; i < MAX_NPC_COUNT; i++) {
    //     if (enemies[i].isInit) {
    //         enemies[i].tick(&enemies[i], calc_collide, delay);
    //     }
    // }

    camera.x = player.position.x + player_x_offset;
    camera.y = player.position.y + player_y_offset;

    return GAME;
}

void game_updateFrame()
{
    background_draw_sky();

    OAM[0] = oam_object_backbuffer[0];

    // player_draw(&player, &camera);

    // for(uint_fast16_t i = 0; i < MAX_NPC_COUNT; i++) {
    //     if (enemies[i].isInit) {
    //         enemies[i].draw(&enemies[i], &camera, enem_sprites);
    //     }
    // }
}

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources
};
