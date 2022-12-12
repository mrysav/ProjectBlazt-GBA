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
  PlayerInputComponent input;
  PlayerObject player;
  LevelObject level;

  PullRequestObject pull_requests[32];

  u16 counter = 0;
};
