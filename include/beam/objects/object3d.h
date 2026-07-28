#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"

namespace beam {

/**
 * @brief Base class for 3D game objects.
 *
 * Object3D mirrors GameObject's role for 2D objects (Sprite, Frame, ...):
 * it keeps its own transform (position/rotation/scale/tint) separate from
 * Node's 2D `position`, since that field is only meaningful for 2D
 * placement. Concrete 3D objects (e.g. Cube3D) derive from this and are
 * expected to be drawn while a Camera3D's 3D mode is active.
 */
class Object3D : public GameObject {
protected:
  Vector3 position;
  Vector3 rotationAxis;
  float rotationAngle;
  Vector3 scale;
  Color tint;

public:
  /**
   * @brief Construct a 3D object.
   * @param name Node name (see Node::getName).
   * @param position World-space position.
   * @param rotationAxis Axis to rotate `rotationAngle` degrees around, as
   *   used by raylib's DrawModelEx/DrawCubeWiresV-style APIs.
   * @param rotationAngle Rotation around `rotationAxis`, in degrees.
   * @param scale Per-axis scale factor.
   * @param tint Color multiplier applied when drawing.
   */
  Object3D(const std::string &name, Vector3 position,
           Vector3 rotationAxis = {0.0f, 1.0f, 0.0f}, float rotationAngle = 0.0f,
           Vector3 scale = {1.0f, 1.0f, 1.0f}, Color tint = WHITE);

  /// @brief Set the world-space position.
  void setPosition(Vector3);
  /// @brief Set the axis rotation is applied around (see getRotationAngle).
  void setRotationAxis(Vector3);
  /// @brief Set the rotation angle around getRotationAxis(), in degrees.
  void setRotationAngle(float);
  /// @brief Set the per-axis scale factor.
  void setScale(Vector3);
  /// @brief Set the color multiplier applied when drawing.
  void setTint(Color);

  /// @brief Get the world-space position.
  Vector3 getPosition() const;
  /// @brief Get the axis rotation is applied around.
  Vector3 getRotationAxis() const;
  /// @brief Get the rotation angle around getRotationAxis(), in degrees.
  float getRotationAngle() const;
  /// @brief Get the per-axis scale factor.
  Vector3 getScale() const;
  /// @brief Get the color multiplier applied when drawing.
  Color getTint() const;

  /**
   * @brief Get this object's axis-aligned world-space bounding box, used by
   * checkRayCollision()/collidesWith(). Default implementation returns a
   * degenerate (zero-size) box at getPosition(); concrete objects with
   * actual extents (Cube3D, Mesh3D, Model3D) override this.
   */
  virtual BoundingBox getBoundingBox() const;

  /**
   * @brief Test a ray against this object's bounding box (see
   * getBoundingBox()). Model3D overrides this to test against its actual
   * mesh geometry instead (via GetRayCollisionMesh).
   */
  virtual RayCollision checkRayCollision(Ray ray) const;

  /// @brief Test whether this object's bounding box overlaps another's.
  bool collidesWith(const Object3D &other) const;
};

} // namespace beam
