#pragma once

enum StateType { MENU = 0, GAME = 1 };

// typedef struct GameState {
//   void (*loadResources)();
//   State (*processInput)(uint16_t keys);
//   void (*updateDisplay)();
//   void (*unloadResources)();
// } GameState;

class State {
public:
  virtual void load(){};
  virtual StateType update(){};
  virtual void unload(){};
};
