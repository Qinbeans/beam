#pragma once

#include "beam/core/manager.h"
#include "beam/objects/object3d.h"
#include "raylib.h"
#include <functional>

namespace beam {

/**
 * @brief A simple axis-aligned 3D box, drawn with raylib's DrawCubeV/
 * DrawCubeWiresV. Intended as a lightweight example 3D game object; expects
 * to be drawn while a Camera3D's 3D mode is active.
 */
class Cube3D : public Object3D {
private:
  Vector3 size;
  bool wireframe;
  Color wireColor;

  std::function<void(float, Cube3D *, SharedManager)> updateCallback;
  std::function<void(Cube3D *, SharedManager)> drawCallback;

public:
  /**
   * @brief Construct a cube.
   * @param name Node name.
   * @param position World-space center of the cube.
   * @param size Width/height/length of the cube along each axis.
   * @param tint Fill color.
   * @param wireframe Whether to also draw a wireframe outline.
   * @param wireColor Wireframe outline color, used when `wireframe` is true.
   */
  Cube3D(const std::string &name, Vector3 position, Vector3 size,
         Color tint = WHITE, bool wireframe = false, Color wireColor = BLACK);

  /// @brief Draw the cube (and its wireframe, if enabled) and invoke the
  /// draw callback, if any.
  void draw(SharedManager) override;
  /// @brief Advance state and invoke the update callback, if any.
  void update(float, SharedManager) override;

  /// @brief Set the cube's width/height/length along each axis.
  void setSize(Vector3);
  /// @brief Enable/disable drawing a wireframe outline.
  void setWireframe(bool);
  /// @brief Set the wireframe outline color.
  void setWireColor(Color);

  /// @brief Get the cube's width/height/length along each axis.
  Vector3 getSize() const;
  /// @brief Whether a wireframe outline is drawn.
  bool isWireframe() const;
  /// @brief Get the wireframe outline color.
  Color getWireColor() const;

  /// @brief Get the cube's axis-aligned world-space bounding box (centered
  /// on getPosition(), extents from getSize()).
  BoundingBox getBoundingBox() const override;

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, Cube3D *, SharedManager)>);
  /// @brief Register a callback invoked at the end of every draw().
  void onDraw(std::function<void(Cube3D *, SharedManager)>);
};

} // namespace beam
