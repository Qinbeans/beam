#pragma once

#include "beam/core/manager.h"
#include "beam/objects/object3d.h"
#include "raylib.h"
#include <functional>
#include <string>

namespace beam {

/**
 * @brief A generated 3D primitive (sphere/cylinder/cone/torus/plane/...),
 * drawn with raylib's DrawMesh. Complements Cube3D for shapes raylib can
 * generate procedurally via its GenMesh* functions.
 *
 * Unlike Cube3D (which draws directly with DrawCubeV, no GPU buffers of its
 * own), a Mesh needs a VAO/VBO uploaded to the GPU, so Mesh3D caches its
 * ::Mesh/::Material on the Manager (keyed by `cacheKey`) the same way
 * Sprite caches its texture - constructing multiple Mesh3D instances with
 * the same `cacheKey` reuses one set of GPU buffers instead of generating
 * (and uploading) duplicates.
 */
class Mesh3D : public Object3D {
private:
  Mesh mesh;
  Material material;
  bool wireframe;

  std::function<void(float, Mesh3D *, SharedManager)> updateCallback;
  std::function<void(Mesh3D *, SharedManager)> drawCallback;

public:
  /**
   * @brief Construct (or reuse a cached) mesh/material and place it in the
   * world.
   * @param manager Manager used to cache the mesh/material.
   * @param name Node name.
   * @param cacheKey Cache key identifying this shape's geometry; give
   *   distinct shapes/parameters distinct keys (e.g.
   *   "sphere-r1.0-rings16-slices16") so they don't share buffers.
   *   `generatedMesh` is only used the first time a key is seen.
   * @param generatedMesh A mesh built with one of raylib's GenMesh*
   *   functions (GenMeshSphere, GenMeshCylinder, GenMeshCone, GenMeshTorus,
   *   GenMeshPlane, ...), or any other valid ::Mesh.
   * @param position World-space position.
   * @param rotationAxis Axis to rotate `rotationAngle` degrees around.
   * @param rotationAngle Rotation around `rotationAxis`, in degrees.
   * @param scale Per-axis scale factor.
   * @param tint Color multiplier applied when drawing (sets the default
   *   material's diffuse color).
   * @param wireframe Whether to draw a wireframe overlay instead of/besides
   *   the solid mesh (drawn via rlgl's wireframe mode is not exposed here;
   *   toggling `wireframe` swaps to raylib's default wireframe rendering of
   *   the same mesh via DrawMesh with a wire-mode material color instead).
   */
  Mesh3D(SharedManager manager, const std::string &name,
         const std::string &cacheKey, Mesh generatedMesh, Vector3 position,
         Vector3 rotationAxis = {0.0f, 1.0f, 0.0f}, float rotationAngle = 0.0f,
         Vector3 scale = {1.0f, 1.0f, 1.0f}, Color tint = WHITE,
         bool wireframe = false);

  /// @brief Draw the mesh with its material at the current transform, then
  /// invoke the draw callback, if any.
  void draw(SharedManager) override;
  /// @brief Advance state and invoke the update callback, if any.
  void update(float, SharedManager) override;

  /// @brief Enable/disable wireframe rendering.
  void setWireframe(bool wireframe);
  /// @brief Whether wireframe rendering is enabled.
  bool isWireframe() const;

  /// @brief Get the mesh's axis-aligned bounding box, translated to its
  /// current world position.
  BoundingBox getBoundingBox() const override;

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, Mesh3D *, SharedManager)>);
  /// @brief Register a callback invoked at the end of every draw().
  void onDraw(std::function<void(Mesh3D *, SharedManager)>);
};

} // namespace beam
