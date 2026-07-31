#include "beam/objects/cube3d.h"

// raymath.h's helper functions use brace-initializers (e.g. `Matrix result =
// { 0 };`) that trip -Wmissing-field-initializers under beam's warning
// flags; that's a raymath.h authoring style, not a beam issue, so silence it
// locally around this vendored header only.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#include "raymath.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include "rlgl.h"

namespace beam {

Cube3D::Cube3D(const std::string &name, Vector3 position, Vector3 size,
               Color tint, bool wireframe, Color wireColor)
    : Object3D(name, position, {0.0f, 1.0f, 0.0f}, 0.0f, {1.0f, 1.0f, 1.0f},
               tint),
      size(size), wireframe(wireframe), wireColor(wireColor) {}

void Cube3D::draw(SharedManager manager) {
  GameObject::draw(manager);
  // Camera3D draws its children unconditionally, so without this setActive
  // (false) would set the flag and change nothing on screen.
  if (!isActive())
    return;

  // DrawCubeV/DrawCubeWiresV take a position but no matrix, so they can't
  // express rotation, scale or a parent's transform on their own. Pushing the
  // world matrix onto rlgl's stack and drawing at the origin under it gets all
  // of that while keeping DrawCubeWiresV's clean twelve-edge outline, which a
  // triangle-mesh wireframe would clutter with face diagonals.
  rlPushMatrix();
  rlMultMatrixf(MatrixToFloat(getWorldMatrix()));

  DrawCubeV(Vector3{0.0f, 0.0f, 0.0f}, size, tint);
  if (wireframe) {
    DrawCubeWiresV(Vector3{0.0f, 0.0f, 0.0f}, size, wireColor);
  }

  rlPopMatrix();

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
  return transformBoundingBox(
      BoundingBox{Vector3{-half.x, -half.y, -half.z},
                  Vector3{half.x, half.y, half.z}});
}

void Cube3D::onUpdate(
    std::function<void(float, Cube3D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Cube3D::onDraw(std::function<void(Cube3D *, SharedManager)> callback) {
  drawCallback = callback;
}

} // namespace beam
