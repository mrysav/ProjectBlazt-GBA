#pragma once

class Rectangle {
public:
  Rectangle(){};
  Rectangle(int x, int y, int width, int height)
      : _x(x), _y(y), _width(width), _height(height){};

  int &x();
  void set_x(int x);

  int &y();
  void set_y(int y);

  int &width();
  void set_width(int width);

  int &height();
  void set_height(int height);

  bool collides(Rectangle &other);

  bool contains(int x, int y);
  bool contains(Rectangle &other);

private:
  int _x;
  int _y;
  int _width;
  int _height;
};
