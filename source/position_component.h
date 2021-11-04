#pragma once

class PositionComponent {
public:
  void set_x(int x);
  void set_y(int y);

  int get_x();
  int get_y();

private:
  int x;
  int y;
};
