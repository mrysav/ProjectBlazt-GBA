#include <gba_input.h>

#include "position_component.h"

void PositionComponent::set_x(int x) { this->x = x; }
void PositionComponent::set_y(int y) { this->y = y; }

int PositionComponent::get_x() { return this->x; }
int PositionComponent::get_y() { return this->y; }
