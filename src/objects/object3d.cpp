#include "beam/objects/object3d.h"

namespace beam {

Object3D::Object3D(const std::string &name, Vector3 position,
                    Vector3 rotationAxis, float rotationAngle, Vector3 scale,
                    Color tint)
    : GameObject(name), position(position), rotationAxis(rotationAxis),
      rotationAngle(rotationAngle), scale(scale), tint(tint) {}

void Object3D::setPosition(Vector3 position) { this->position = position; }

void Object3D::setRotationAxis(Vector3 rotationAxis) {
  this->rotationAxis = rotationAxis;
}

void Object3D::setRotationAngle(float rotationAngle) {
  this->rotationAngle = rotationAngle;
}

void Object3D::setScale(Vector3 scale) { this->scale = scale; }

void Object3D::setTint(Color tint) { this->tint = tint; }

Vector3 Object3D::getPosition() const { return position; }

Vector3 Object3D::getRotationAxis() const { return rotationAxis; }

float Object3D::getRotationAngle() const { return rotationAngle; }

Vector3 Object3D::getScale() const { return scale; }

Color Object3D::getTint() const { return tint; }

BoundingBox Object3D::getBoundingBox() const {
  return BoundingBox{position, position};
}

RayCollision Object3D::checkRayCollision(Ray ray) const {
  return GetRayCollisionBox(ray, getBoundingBox());
}

bool Object3D::collidesWith(const Object3D &other) const {
  return CheckCollisionBoxes(getBoundingBox(), other.getBoundingBox());
}

} // namespace beam
