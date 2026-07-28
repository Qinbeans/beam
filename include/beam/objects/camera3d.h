#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

namespace beam {

/**
 * @brief A 3D camera, wrapping raylib's Camera3D.
 *
 * Beam never exposes the underlying ::Camera3D struct directly; instead
 * position/target/up/fovy/projection are manipulated through this object's
 * own setters and getters, following the same paradigm as the rest of
 * beam's GameObjects (e.g. Sprite wrapping Vector2/Rectangle/Color state).
 *
 * Camera3D behaves like Frame: it owns a buffer of child GameObjects (meant
 * to be Object3D-derived, e.g. Cube3D) added via `operator<<`, and draws
 * them inside a BeginMode3D/EndMode3D block so they're rendered with this
 * camera's projection.
 *
 * `projection` and `mode` use raylib's own CameraProjection/CameraMode int
 * constants (CAMERA_PERSPECTIVE/CAMERA_ORTHOGRAPHIC and
 * CAMERA_CUSTOM/CAMERA_FREE/CAMERA_ORBITAL/CAMERA_FIRST_PERSON/
 * CAMERA_THIRD_PERSON), matching how the rest of beam passes through
 * raylib enums (e.g. Manager::isKeyDown(int key)). When mode is not
 * CAMERA_CUSTOM, update() drives the camera every frame via raylib's
 * UpdateCamera().
 */
class Camera3D : public GameObject {
private:
  ::Camera3D camera;
  int mode;

  std::vector<std::shared_ptr<GameObject>> buffer;

  std::function<void(float, Camera3D *, SharedManager)> updateCallback;
  std::function<void(Camera3D *, SharedManager)> drawCallback;

public:
  /**
   * @brief Construct a 3D camera.
   * @param name Node name.
   * @param position Camera's world-space position (eye).
   * @param target Point the camera looks at.
   * @param up Camera's up vector (rotation around its own axis).
   * @param fovy Field-of-view aperture in Y, in degrees for perspective, or
   *   near-plane width for orthographic.
   * @param projection Either CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC.
   * @param mode One of raylib's CameraMode constants (CAMERA_CUSTOM,
   *   CAMERA_FREE, CAMERA_ORBITAL, CAMERA_FIRST_PERSON,
   *   CAMERA_THIRD_PERSON). CAMERA_CUSTOM (the default) leaves the camera
   *   fully under caller control; any other mode drives it every frame via
   *   raylib's UpdateCamera() in update().
   */
  Camera3D(const std::string &name, Vector3 position, Vector3 target,
           Vector3 up = {0.0f, 1.0f, 0.0f}, float fovy = 45.0f,
           int projection = CAMERA_PERSPECTIVE, int mode = CAMERA_CUSTOM);

  /**
   * @brief Add a child GameObject to be drawn inside this camera's 3D mode
   * (BeginMode3D/EndMode3D). Mirrors Frame/Scene's `operator<<`.
   */
  template <typename T> Camera3D &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  /// @brief Initialize all child GameObjects.
  void init(SharedManager) override;
  /**
   * @brief Drive the camera (via raylib's UpdateCamera(), when `mode` is not
   * CAMERA_CUSTOM), update all children, then invoke the update callback,
   * if any.
   */
  void update(float, SharedManager) override;
  /**
   * @brief Draw all children inside a BeginMode3D/EndMode3D block using this
   * camera's current state, then invoke the draw callback, if any.
   */
  void draw(SharedManager) override;

  /// @brief Set the camera's world-space position (eye).
  void setPosition(Vector3);
  /// @brief Set the point the camera looks at.
  void setTarget(Vector3);
  /// @brief Set the camera's up vector.
  void setUp(Vector3);
  /// @brief Set the field-of-view aperture in Y (degrees for perspective,
  /// near-plane width for orthographic).
  void setFovy(float);
  /// @brief Set the projection mode (CAMERA_PERSPECTIVE/CAMERA_ORTHOGRAPHIC).
  void setProjection(int);
  /// @brief Set the raylib CameraMode driving automatic updates.
  void setMode(int);

  /// @brief Get the camera's world-space position (eye).
  Vector3 getPosition() const;
  /// @brief Get the point the camera looks at.
  Vector3 getTarget() const;
  /// @brief Get the camera's up vector.
  Vector3 getUp() const;
  /// @brief Get the field-of-view aperture in Y.
  float getFovy() const;
  /// @brief Get the projection mode.
  int getProjection() const;
  /// @brief Get the raylib CameraMode driving automatic updates.
  int getMode() const;

  /// @brief Get the camera's view transform matrix (raylib's
  /// GetCameraMatrix).
  Matrix getViewMatrix() const;
  /// @brief Project a 3D world-space point to 2D screen space.
  Vector2 worldToScreen(Vector3 point) const;
  /// @brief Get a ray from the camera through a screen-space point (e.g. for
  /// mouse picking).
  Ray screenToWorldRay(Vector2 point) const;

  /// @brief Get a child by index, cast to type T (nullptr if not that type).
  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, Camera3D *, SharedManager)>);
  /// @brief Register a callback invoked at the end of every draw().
  void onDraw(std::function<void(Camera3D *, SharedManager)>);
};

} // namespace beam
