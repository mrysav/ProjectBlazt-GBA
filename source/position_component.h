#pragma once

class PositionComponent {
public:
  PositionComponent(){};
  PositionComponent(int _x, int _y) : x(_x), y(_y){};

  void set_x(int x);
  void set_y(int y);

  int get_x();
  int get_y();

private:
  int x;
  int y;
};
