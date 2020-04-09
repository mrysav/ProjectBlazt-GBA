#include "c_gba.h"
#include "input.h"
#include "dangerous_dave.h"

#include "block_tiles.h"
#include "sky_map_1.h"

#include <string.h>

ObjectAttributes oam_object_backbuffer[128];

#define min(x,y) (x > y ? y : x)
#define max(x,y) (x < y ? y : x)

const int FLOOR_Y = 160-16;
const int GRAVITY = 1;
const int WALK_SPEED = 4;
const int JUMP_VI = -12;

typedef struct
{
    ObjectAttributes* spriteAttribs;
    int facingRight;
    int firstAnimCycleFrame;
    int animFrame;
    int posX;
    int posY;
    int velX;
    int velY;
    int framesInAir;
}HeroSprite;

void InitializeHeroSprite(HeroSprite* sprite, ObjectAttributes* attribs)
{
    sprite->spriteAttribs = attribs;
    sprite->facingRight = 1;
    sprite->firstAnimCycleFrame = 0;
    sprite->animFrame = 0;
    sprite->posX = 0;
    sprite->posY = FLOOR_Y;
    sprite->velX = 0;
    sprite->velY = 0;
    sprite->framesInAir = 0;
}

void updateSpritePosition(HeroSprite* sprite)
{

    if (getKeyState(KEY_LEFT))
    {
        sprite->facingRight = 1;
        sprite->velX = -WALK_SPEED;
    }
    else if (getKeyState(KEY_RIGHT))
    {
        sprite->facingRight = 0;
        sprite->velX = WALK_SPEED;
    }
    else sprite->velX = 0;

    int isMidAir = sprite->posY != FLOOR_Y;

    if (getKeyState(KEY_A))
    {
        if (!isMidAir)
        {
            sprite->velY = JUMP_VI;
            sprite->framesInAir = 0;
        }
    }

    if (isMidAir)
    {
        sprite->velY = JUMP_VI + (GRAVITY * sprite->framesInAir);
        sprite->velY = min(5, sprite->velY);
        sprite->framesInAir++;
    }

    sprite->posX += sprite->velX;
    //clamp to Screen
    sprite->posX = min(240-16, sprite->posX);
    sprite->posX = max(0, sprite->posX);

    sprite->posY += sprite->velY;
    sprite->posY = min(sprite->posY, FLOOR_Y);

    sprite->spriteAttribs->attr0 = 0x2000 + sprite->posY;
    sprite->spriteAttribs->attr1 = (sprite->facingRight? 0x4000 : 0x5000) + sprite->posX;


}

void tickSpriteAnimation(HeroSprite* sprite, int frame)
{
    if (frame % 8 != 0) {
        return;
    }

    ObjectAttributes* spriteAttribs = sprite->spriteAttribs;

    int isMidAir = sprite->posY != FLOOR_Y;

    //update velocity for gravity
    if (isMidAir)
    {
        sprite->firstAnimCycleFrame = 32;
        sprite->animFrame = 0;
    }
    else
    {
        if (sprite->velX != 0)
        {
            sprite->firstAnimCycleFrame = 32;
            sprite->animFrame = (++sprite->animFrame) % 3;

        }
    }

    spriteAttribs->attr2 = sprite->firstAnimCycleFrame + (sprite->animFrame * 8);

}

int old_main()
{

    memcpy(&MEM_TILE[4][0], dangerous_daveTiles, dangerous_daveTilesLen);
    memcpy(MEM_PALETTE, dangerous_davePal, dangerous_davePalLen);

    REG_DISPLAYCONTROL =  VIDEOMODE_0 | BACKGROUND_0 | ENABLE_OBJECTS | MAPPINGMODE_1D;

    HeroSprite sprite;
    InitializeHeroSprite(&sprite, &oam_object_backbuffer[0]);

    int frame = 0;
    while(1)
	{
        key_poll();
        vsync();

        updateSpritePosition(&sprite);
        tickSpriteAnimation(&sprite, frame);

        MEM_OAM[0] = oam_object_backbuffer[0];

        // ooft
        frame = frame + 1 % 60;
	}

    return 0;
}
