#include <gba.h>
#include <gba_console.h>
#include <gba_input.h>

#include <stdio.h>

#include "menu.h"
#include "state.h"

using namespace std;

void MenuState::load() {
  consoleDemoInit();

  // ansi escape sequence to clear screen and home cursor
  // /x1b[line;columnH
  iprintf("\x1b[2J");

  // /x1b[line;columnH
  iprintf("\x1b[1;10HOH SNAP!");

  iprintf("\x1b[3;5HA new vulnerability");
  iprintf("\x1b[4;8Hjust dropped!");

  iprintf("\x1b[8;1HDependabot Dave's gotta");
  iprintf("\x1b[9;1Hmerge those pull requests");
  iprintf("\x1b[10;1Hbefore the baddies poison");
  iprintf("\x1b[11;1Hthe supply chain!!");

  // ansi escape sequence to set print co-ordinates
  // /x1b[line;columnH
  iprintf("\x1b[16;7HPress A to Play!");
}

StateType MenuState::update() {
  auto keys = keysUp();

  if (keys & KEY_A) {
    pressed = true;
    return GAME;
  }

  return MENU;
}

void MenuState::unload() {}
