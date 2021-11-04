#pragma once

#include "state.h"

class MenuState : public State {
public:
  void load();
  StateType update();
  void unload();

private:
  bool pressed = false;
};
