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
 * @brief A 2D camera, wrapping raylib's Camera2D.
 *
 * Like Camera3D, the underlying ::Camera2D struct is never exposed
 * directly; offset/target/rotation/zoom are manipulated through this
 * object's own setters and getters. Camera2D owns a buffer of child
 * GameObjects added via `operator<<` (following the Frame paradigm) and
 * draws them inside a BeginMode2D/EndMode2D block, so children are
 * rendered in this camera's transformed space.
 */
class Camera2D : public GameObject {
private:
  ::Camera2D camera;

  std::vector<std::shared_ptr<GameObject>> buffer;

  std::function<void(float, Camera2D *, SharedManager)> updateCallback;
  std::function<void(Camera2D *, SharedManager)> drawCallback;

public:
  /**
   * @brief Construct a 2D camera.
   * @param name Node name.
   * @param offset Displacement from the target on-screen (typically screen
   *   center, so the target appears centered).
   * @param target World-space point the camera is centered on (rotation and
   *   zoom origin).
   * @param rotation Camera rotation, in degrees.
   * @param zoom Camera zoom factor; 1.0 is unscaled.
   */
  Camera2D(const std::string &name, Vector2 offset, Vector2 target,
           float rotation = 0.0f, float zoom = 1.0f);

  /**
   * @brief Add a child GameObject to be drawn inside this camera's 2D mode
   * (BeginMode2D/EndMode2D). Mirrors Frame/Scene's `operator<<`.
   */
  template <typename T> Camera2D &operator<<(std::shared_ptr<T> child) {
    static_assert(std::is_base_of<Node, T>::value,
                  "Child must be derived from Node");
    buffer.push_back(child);
    child->setParent(shared_from_this());
    return *this;
  }

  /// @brief Initialize all child GameObjects.
  void init(SharedManager) override;
  /// @brief Update all children, then invoke the update callback, if any.
  void update(float, SharedManager) override;
  /**
   * @brief Draw all children inside a BeginMode2D/EndMode2D block using this
   * camera's current state, then invoke the draw callback, if any.
   */
  void draw(SharedManager) override;

  /// @brief Set the displacement from the target on-screen.
  void setOffset(Vector2);
  /// @brief Set the world-space point the camera is centered on.
  void setTarget(Vector2);
  /// @brief Set the camera rotation, in degrees.
  void setRotation(float);
  /// @brief Set the camera zoom factor.
  void setZoom(float);

  /// @brief Get the displacement from the target on-screen.
  Vector2 getOffset() const;
  /// @brief Get the world-space point the camera is centered on.
  Vector2 getTarget() const;
  /// @brief Get the camera rotation, in degrees.
  float getRotation() const;
  /// @brief Get the camera zoom factor.
  float getZoom() const;

  /// @brief Get the camera's view transform matrix (raylib's
  /// GetCameraMatrix2D).
  Matrix getViewMatrix() const;
  /// @brief Convert a 2D world-space point to screen space.
  Vector2 worldToScreen(Vector2 point) const;
  /// @brief Convert a 2D screen-space point to world space.
  Vector2 screenToWorld(Vector2 point) const;

  /// @brief Get a child by index, cast to type T (nullptr if not that type).
  template <typename T> T *getChildAs(int index) {
    return dynamic_cast<T *>(buffer[index].get());
  }

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, Camera2D *, SharedManager)>);
  /// @brief Register a callback invoked at the end of every draw().
  void onDraw(std::function<void(Camera2D *, SharedManager)>);
};

} // namespace beam
