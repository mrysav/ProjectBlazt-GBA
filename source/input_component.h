#pragma once

#include <gba_types.h>

class InputComponent {
public:
  virtual void update(u16 counter);

  virtual bool down(u16 keys);
  virtual bool up(u16 keys);
  virtual bool held(u16 keys);
};
