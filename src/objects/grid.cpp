#include "beam/objects/grid.h"
#include "beam/objects/game_object.h"
#include "raygui.h"

namespace beam {

const Rectangle Grid::getBounds() const {
  return {position.x, position.y, size.x, size.y};
}

void Grid::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!active) {
    return;
  }

  int prevLineColor = GuiGetStyle(DEFAULT, LINE_COLOR);
  GuiSetStyle(DEFAULT, LINE_COLOR, ColorToInt(color));

  GuiGrid(getBounds(), nullptr, spacing, subdivisions, &mouseCell);

  GuiSetStyle(DEFAULT, LINE_COLOR, prevLineColor);
}

void Grid::setPosition(Vector2 position) { this->position = position; }

void Grid::setSize(Vector2 size) { this->size = size; }

void Grid::setSpacing(float spacing) { this->spacing = spacing; }

void Grid::setSubdivisions(int subdivisions) { this->subdivisions = subdivisions; }

void Grid::setColor(Color color) { this->color = color; }

Vector2 Grid::getPosition() const { return position; }

Vector2 Grid::getSize() const { return size; }

float Grid::getSpacing() const { return spacing; }

int Grid::getSubdivisions() const { return subdivisions; }

Color Grid::getColor() const { return color; }

Vector2 Grid::getMouseCell() const { return mouseCell; }

} // namespace beam
