#pragma once

enum StateType { MENU = 0, GAME = 1 };

class State {
public:
  virtual void load(){};
  virtual StateType update(){};
  virtual void unload(){};
};
