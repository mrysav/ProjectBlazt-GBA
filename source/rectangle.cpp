#include "rectangle.h"

int &Rectangle::x() { return _x; }

void Rectangle::set_x(int x) { _x = x; }

int &Rectangle::y() { return _y; }

void Rectangle::set_y(int y) { _y = y; }

int &Rectangle::width() { return _width; }

void Rectangle::set_width(int width) { _width = width; }

int &Rectangle::height() { return _height; }

void Rectangle::set_height(int height) { _height = height; }

bool Rectangle::collides(Rectangle &other) {
  return (_x < other._x + other._width && _x + _width > other._x &&
          _y < other._y + other._height && _height + _y > other._y);
}

bool Rectangle::contains(int x, int y) {
  return (x >= _x) && (x <= _x + _width) && (y >= _y) && (y <= _y + _height);
}
bool Rectangle::contains(Rectangle &other) {
  return contains(other._x, other._y) &&
         contains(other._x + other._width, other._y + other._height);
}
