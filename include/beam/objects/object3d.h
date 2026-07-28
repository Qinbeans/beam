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
  Vector3 pivot;
  Color tint;

  /**
   * @brief Expand a local-space bounding box into the axis-aligned world-space
   * box that encloses it under getWorldMatrix(). Subclasses that know their
   * local extents (Cube3D, Mesh3D, Model3D) build their getBoundingBox() on
   * this rather than just offsetting by position, so a rotated or parented
   * object still reports a box that actually contains it.
   */
  BoundingBox transformBoundingBox(BoundingBox local) const;

public:
  /// @brief Maximum ancestor chain length walked by getWorldMatrix(). Bounds
  /// the walk so an accidental parent cycle fails visibly instead of hanging.
  static constexpr int MAX_TRANSFORM_DEPTH = 64;

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

  /// @brief Set the position, relative to the parent Object3D if there is one
  /// (see getWorldMatrix()), otherwise in world space.
  void setPosition(Vector3);
  /// @brief Set the axis rotation is applied around (see getRotationAngle).
  void setRotationAxis(Vector3);
  /// @brief Set the rotation angle around getRotationAxis(), in degrees.
  void setRotationAngle(float);
  /// @brief Set the per-axis scale factor.
  void setScale(Vector3);
  /// @brief Set the color multiplier applied when drawing.
  void setTint(Color);

  /**
   * @brief Set the local-space point that rotation and scale happen around,
   * and that getPosition() places. Defaults to the origin, which leaves
   * behaviour identical to a plain position/rotation/scale object.
   *
   * This is what makes jointed animation work without a skeleton: give a limb
   * a pivot at its joint (e.g. the shoulder end of an arm), parent it to the
   * torso, and setRotationAngle()/setEuler() swing it about that joint the way
   * a bone would - which is how voxel characters are animated in practice.
   */
  void setPivot(Vector3);

  /**
   * @brief Set the rotation as intrinsic Euler angles in degrees, where
   * `pitch` turns around X, `yaw` around Y and `roll` around Z. Stored
   * internally as the equivalent axis/angle, so getRotationAxis() and
   * getRotationAngle() stay meaningful afterwards.
   */
  void setEuler(float pitch, float yaw, float roll);

  /// @brief Get the world-space position.
  Vector3 getPosition() const;
  /// @brief Get the axis rotation is applied around.
  Vector3 getRotationAxis() const;
  /// @brief Get the rotation angle around getRotationAxis(), in degrees.
  float getRotationAngle() const;
  /// @brief Get the per-axis scale factor.
  Vector3 getScale() const;
  /// @brief Get the local-space point rotation and scale happen around.
  Vector3 getPivot() const;
  /// @brief Get the rotation as Euler angles in degrees (see setEuler).
  Vector3 getEuler() const;
  /// @brief Get the color multiplier applied when drawing.
  Color getTint() const;

  /**
   * @brief Get this object's own transform: pivot, then scale, then rotation,
   * then position. Does not include any parent's transform.
   */
  Matrix getLocalMatrix() const;

  /**
   * @brief Get getLocalMatrix() composed with the local matrix of every
   * Object3D above this one in the Node tree, nearest ancestor first. This is
   * the matrix everything actually draws with.
   *
   * Note that parenting only composes transforms - it does not draw. Node
   * keeps a parent pointer but no child list, so only container nodes
   * (Camera3D, Scene, ...) render their children. A limb hierarchy is
   * therefore built by adding every part to the Camera3D for drawing and
   * calling setParent() to say how the parts move together.
   *
   * Non-Object3D nodes in the chain are skipped, so an Object3D nested under
   * a plain Node under another Object3D still composes correctly.
   */
  Matrix getWorldMatrix() const;

  /// @brief Get the translation component of getWorldMatrix() - i.e. where
  /// this object actually ends up once its parents are taken into account.
  Vector3 getWorldPosition() const;

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
