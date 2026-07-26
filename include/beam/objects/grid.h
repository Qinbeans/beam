#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"

namespace beam {
class Grid : public GameObject {
protected:
  Vector2 position;
  Vector2 size;
  float spacing;
  int subdivisions;
  Color color;
  Vector2 mouseCell;

public:
  Grid(Vector2 position, Vector2 size, float spacing = 16.0f,
       int subdivisions = 4, Color color = LIGHTGRAY)
      : position(position), size(size), spacing(spacing),
        subdivisions(subdivisions), color(color), mouseCell{-1, -1} {}

  void draw(SharedManager) override;

  void setPosition(Vector2 position);
  void setSize(Vector2 size);
  void setSpacing(float spacing);
  void setSubdivisions(int subdivisions);
  void setColor(Color color);

  Vector2 getPosition() const;
  Vector2 getSize() const;
  float getSpacing() const;
  int getSubdivisions() const;
  Color getColor() const;
  Vector2 getMouseCell() const;

  const Rectangle getBounds() const;
};
} // namespace beam
