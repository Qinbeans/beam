#include "beam/objects/cube3d.h"

namespace beam {

Cube3D::Cube3D(const std::string &name, Vector3 position, Vector3 size,
               Color tint, bool wireframe, Color wireColor)
    : Object3D(name, position, {0.0f, 1.0f, 0.0f}, 0.0f, {1.0f, 1.0f, 1.0f},
               tint),
      size(size), wireframe(wireframe), wireColor(wireColor) {}

void Cube3D::draw(SharedManager manager) {
  GameObject::draw(manager);
  DrawCubeV(position, size, tint);
  if (wireframe) {
    DrawCubeWiresV(position, size, wireColor);
  }
  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Cube3D::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Cube3D::setSize(Vector3 size) { this->size = size; }

void Cube3D::setWireframe(bool wireframe) { this->wireframe = wireframe; }

void Cube3D::setWireColor(Color wireColor) { this->wireColor = wireColor; }

Vector3 Cube3D::getSize() const { return size; }

bool Cube3D::isWireframe() const { return wireframe; }

Color Cube3D::getWireColor() const { return wireColor; }

BoundingBox Cube3D::getBoundingBox() const {
  Vector3 half = {size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
  return BoundingBox{
      Vector3{position.x - half.x, position.y - half.y, position.z - half.z},
      Vector3{position.x + half.x, position.y + half.y, position.z + half.z}};
}

void Cube3D::onUpdate(
    std::function<void(float, Cube3D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Cube3D::onDraw(std::function<void(Cube3D *, SharedManager)> callback) {
  drawCallback = callback;
}

} // namespace beam
