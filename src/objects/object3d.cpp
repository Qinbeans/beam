#include "beam/objects/object3d.h"

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

#include <algorithm>
#include <memory>

namespace beam {

Object3D::Object3D(const std::string &name, Vector3 position,
                    Vector3 rotationAxis, float rotationAngle, Vector3 scale,
                    Color tint)
    : GameObject(name), position(position), rotationAxis(rotationAxis),
      rotationAngle(rotationAngle), scale(scale), pivot{0.0f, 0.0f, 0.0f},
      tint(tint) {}

void Object3D::setPosition(Vector3 position) { this->position = position; }

void Object3D::setRotationAxis(Vector3 rotationAxis) {
  this->rotationAxis = rotationAxis;
}

void Object3D::setRotationAngle(float rotationAngle) {
  this->rotationAngle = rotationAngle;
}

void Object3D::setScale(Vector3 scale) { this->scale = scale; }

void Object3D::setPivot(Vector3 pivot) { this->pivot = pivot; }

void Object3D::setEuler(float pitch, float yaw, float roll) {
  Quaternion rotation =
      QuaternionFromEuler(pitch * DEG2RAD, yaw * DEG2RAD, roll * DEG2RAD);

  Vector3 axis{0.0f, 0.0f, 0.0f};
  float angle = 0.0f;
  QuaternionToAxisAngle(rotation, &axis, &angle);

  // A zero rotation has no meaningful axis; QuaternionToAxisAngle reports an
  // arbitrary one, so keep beam's default instead of storing that.
  if (Vector3LengthSqr(axis) < EPSILON) {
    axis = Vector3{0.0f, 1.0f, 0.0f};
    angle = 0.0f;
  }

  rotationAxis = axis;
  rotationAngle = angle * RAD2DEG;
}

void Object3D::setTint(Color tint) { this->tint = tint; }

Vector3 Object3D::getPosition() const { return position; }

Vector3 Object3D::getRotationAxis() const { return rotationAxis; }

float Object3D::getRotationAngle() const { return rotationAngle; }

Vector3 Object3D::getScale() const { return scale; }

Vector3 Object3D::getPivot() const { return pivot; }

Vector3 Object3D::getEuler() const {
  Quaternion rotation =
      QuaternionFromAxisAngle(rotationAxis, rotationAngle * DEG2RAD);
  Vector3 euler = QuaternionToEuler(rotation);
  return Vector3{euler.x * RAD2DEG, euler.y * RAD2DEG, euler.z * RAD2DEG};
}

Color Object3D::getTint() const { return tint; }

Matrix Object3D::getLocalMatrix() const {
  // raymath composes left-to-right in application order, so this reads as:
  // move the pivot to the origin, scale, rotate about it, then translate the
  // pivot to `position`.
  return MatrixMultiply(
      MatrixMultiply(
          MatrixMultiply(MatrixTranslate(-pivot.x, -pivot.y, -pivot.z),
                          MatrixScale(scale.x, scale.y, scale.z)),
          MatrixRotate(rotationAxis, rotationAngle * DEG2RAD)),
      MatrixTranslate(position.x, position.y, position.z));
}

Matrix Object3D::getWorldMatrix() const {
  Matrix result = getLocalMatrix();

  // Walk up rather than recurse: an ancestor's getWorldMatrix() would redo
  // the whole chain above it at every level. Non-Object3D nodes are skipped
  // so a plain Node in between doesn't sever the relationship.
  std::shared_ptr<Node> node = parent.lock();
  for (int depth = 0; node && depth < MAX_TRANSFORM_DEPTH; depth++) {
    if (auto ancestor = std::dynamic_pointer_cast<Object3D>(node)) {
      result = MatrixMultiply(result, ancestor->getLocalMatrix());
    }
    node = node->getParent().lock();
  }

  return result;
}

Vector3 Object3D::getWorldPosition() const {
  Matrix world = getWorldMatrix();
  return Vector3{world.m12, world.m13, world.m14};
}

BoundingBox Object3D::transformBoundingBox(BoundingBox local) const {
  Matrix world = getWorldMatrix();

  // An AABB isn't closed under rotation, so transform all eight corners and
  // take the extents of the result rather than just the two stored points.
  const Vector3 corners[8] = {
      Vector3{local.min.x, local.min.y, local.min.z},
      Vector3{local.min.x, local.min.y, local.max.z},
      Vector3{local.min.x, local.max.y, local.min.z},
      Vector3{local.min.x, local.max.y, local.max.z},
      Vector3{local.max.x, local.min.y, local.min.z},
      Vector3{local.max.x, local.min.y, local.max.z},
      Vector3{local.max.x, local.max.y, local.min.z},
      Vector3{local.max.x, local.max.y, local.max.z},
  };

  Vector3 first = Vector3Transform(corners[0], world);
  BoundingBox box{first, first};
  for (int i = 1; i < 8; i++) {
    Vector3 corner = Vector3Transform(corners[i], world);
    box.min.x = std::min(box.min.x, corner.x);
    box.min.y = std::min(box.min.y, corner.y);
    box.min.z = std::min(box.min.z, corner.z);
    box.max.x = std::max(box.max.x, corner.x);
    box.max.y = std::max(box.max.y, corner.y);
    box.max.z = std::max(box.max.z, corner.z);
  }

  return box;
}

BoundingBox Object3D::getBoundingBox() const {
  Vector3 origin = getWorldPosition();
  return BoundingBox{origin, origin};
}

RayCollision Object3D::checkRayCollision(Ray ray) const {
  return GetRayCollisionBox(ray, getBoundingBox());
}

bool Object3D::collidesWith(const Object3D &other) const {
  return CheckCollisionBoxes(getBoundingBox(), other.getBoundingBox());
}

} // namespace beam
