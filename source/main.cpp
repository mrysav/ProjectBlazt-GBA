#include <stdbool.h>

#include <gba.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

// Utilities
#include "display.h"
#include "state.h"
#include "util.h"

// Game States
#include "game.h"
#include "menu.h"

int main() {
  // the vblank interrupt must be enabled for VBlankIntrWait() to work
  // since the default dispatcher handles the bios flags no vblank handler
  // is required
  irqInit();
  irqEnable(IRQ_VBLANK);

  MenuState _menu;
  GameState _game;
  State *game_states[2]{&_menu, &_game};

  StateType state = MENU;
  game_states[(int)state]->load();

  while (1) {
    State *gState = game_states[(int)state];

    scanKeys();

    StateType nextState = gState->update();

    // wait for vsync interrupt
    VBlankIntrWait();

    if (state != nextState) {
      State *nState = game_states[(int)nextState];

      gState->unload();
      nState->load();

      state = nextState;
    }
  }

  return 0;
}
