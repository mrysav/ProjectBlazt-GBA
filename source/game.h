#pragma once

#include "level_object.h"
#include "player_object.h"
#include "state.h"

class GameState : public State {
public:
  GameState();

  void load();
  StateType update();
  void unload();

private:
  // Really need to have a global player input
  PlayerInputComponent input = PlayerInputComponent();
  PlayerObject player;
  LevelObject level;

  u16 counter = 0;
};
