#include <stdbool.h>

#include <gba.h>

// Game States
#include "menu.h"
#include "game.h"

// Utilities
#include "state.h"
#include "display.h"
#include "util.h"

const GameState* GAME_STATES[] = { &MENU_STATE, &GAME_STATE };

void vsync()
{
    while (REG_VCOUNT >= 160);
    while (REG_VCOUNT < 160);
}

int main()
{
    REG_DISPCNT =  MODE_0 | BG0_ON | BG1_ON | BG2_ON | OBJ_ON | OBJ_1D_MAP;

    State state = MENU;
    GAME_STATES[(int)state]->loadResources();

    int frame = 0;
    while(1)
	{
        const GameState* gState = GAME_STATES[(int)state];
        State nextState = state;

        scanKeys();
        uint16_t keys = keysHeld();

        nextState = gState->processInput(keys);

        // wait for vsync
        vsync();
        frame = frame + 1 % 60;

        gState->updateDisplay();

        if (state != nextState) {
            const GameState* nState = GAME_STATES[(int)nextState];

            gState->unloadResources();
            nState->loadResources();

            state = nextState;
        }
	}

    return 0;
}
