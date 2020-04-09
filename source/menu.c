#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <gba.h>

#include "menu.h"
#include "state.h"
#include "background.h"

bool pressed = false;

void menu_loadResources() {
    // TODO: Load text?
}

void menu_unloadResources() {
    // maybe unneeded?
}

State menu_processInput(uint16_t keys) {

    if(keys & KEY_A) {
        pressed = true;
    }

    return MENU;
}

void menu_updateFrame() {

    if (!pressed) {
        background_draw_black();
    } else {
        background_draw_sky();
    }

    return;
}

const GameState MENU_STATE = { &menu_loadResources, &menu_processInput, &menu_updateFrame, &menu_unloadResources };
