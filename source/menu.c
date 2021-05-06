#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <gba.h>

#include "menu.h"
#include "state.h"

bool pressed = false;

void menu_loadResources()
{
    // TODO: Load text?
}

void menu_unloadResources()
{
    // maybe unneeded?
}

State menu_processInput(uint16_t keys)
{

    if (keys & KEY_A)
    {
        pressed = true;
        return GAME;
    }

    return MENU;
}

void menu_updateFrame()
{
    return;
}

const GameState MENU_STATE = {&menu_loadResources, &menu_processInput, &menu_updateFrame, &menu_unloadResources};
