#pragma once

#include "beam/core/manager.h"
#include "beam/objects/object3d.h"
#include "raylib.h"
#include <functional>
#include <string>
#include <vector>

namespace beam {

/**
 * @brief A loaded 3D model (meshes + materials + optional skeleton), drawn
 * with raylib's DrawModelEx/DrawModelWiresEx.
 *
 * The underlying ::Model is loaded once per file path and cached on the
 * Manager (keyed by `modelPath + "Model"`), mirroring Sprite's
 * texture-cache pattern, so multiple Model3D instances for the same file
 * share one set of GPU buffers.
 */
class Model3D : public Object3D {
private:
  Model model;
  bool wireframe;

  ModelAnimation *animations;
  int animationCount;

  int fromIndex;
  int toIndex;
  float frameA;
  float frameB;
  float blend;
  bool crossfading;
  float blendDuration;
  float blendElapsed;
  bool loop;
  float speed;
  float fps;
  bool playing;

  std::function<void(float, Model3D *, SharedManager)> updateCallback;
  std::function<void(Model3D *, SharedManager)> drawCallback;

public:
  /**
   * @brief Load (or reuse a cached) model and place it in the world.
   * @param manager Manager used to cache the underlying Model asset.
   * @param name Node name.
   * @param modelPath Path to a model file (obj/gltf/glb/vox/iqm/m3d, per
   *   raylib's LoadModel).
   * @param position World-space position.
   * @param rotationAxis Axis to rotate `rotationAngle` degrees around.
   * @param rotationAngle Rotation around `rotationAxis`, in degrees.
   * @param scale Per-axis scale factor.
   * @param tint Color multiplier applied when drawing.
   * @param wireframe Whether to draw in wireframe instead of solid.
   */
  Model3D(SharedManager manager, const std::string &name,
          const std::string &modelPath, Vector3 position,
          Vector3 rotationAxis = {0.0f, 1.0f, 0.0f}, float rotationAngle = 0.0f,
          Vector3 scale = {1.0f, 1.0f, 1.0f}, Color tint = WHITE,
          bool wireframe = false);

  /// @brief Draw the model at the current transform, then invoke the draw
  /// callback, if any.
  void draw(SharedManager) override;
  /// @brief Advance state and invoke the update callback, if any.
  void update(float, SharedManager) override;

  /// @brief Enable/disable wireframe rendering.
  void setWireframe(bool wireframe);
  /// @brief Whether wireframe rendering is enabled.
  bool isWireframe() const;

  /// @brief Get the model's axis-aligned bounding box, translated to its
  /// current world position.
  BoundingBox getBoundingBox() const override;

  /// @brief Test a ray against the model's actual mesh geometry (nearest
  /// hit across all of the model's meshes), rather than just its bounding
  /// box.
  RayCollision checkRayCollision(Ray ray) const override;

  /**
   * @brief Load (or reuse a cached) set of animation clips for this model.
   * Must be called before playAnimation()/crossfadeTo(). Clips whose
   * skeleton doesn't match this model (per raylib's IsModelAnimationValid)
   * are skipped and won't appear in getAnimationNames().
   * @param manager Manager used to cache the animation clips.
   * @param animationPath Path to a file containing animation clips (e.g.
   *   the same .glb/.iqm/.m3d the model was loaded from), per raylib's
   *   LoadModelAnimations.
   */
  void loadAnimations(SharedManager manager, const std::string &animationPath);

  /// @brief Get the names of all loaded (and valid) animation clips.
  std::vector<std::string> getAnimationNames() const;

  /**
   * @brief Immediately switch to playing an animation clip by name (no
   * blending from the current pose).
   * @param name Clip name, from getAnimationNames().
   * @param loop Whether to loop back to frame 0 when the clip ends.
   * @param speed Playback speed multiplier (1.0 is normal speed).
   */
  void playAnimation(const std::string &name, bool loop = true,
                      float speed = 1.0f);

  /**
   * @brief Smoothly blend from the current animation into another one over
   * `duration` seconds.
   * @param name Clip name to blend into, from getAnimationNames().
   * @param duration Blend duration, in seconds.
   * @param loop Whether the new clip should loop once the blend completes.
   * @param speed Playback speed multiplier for the new clip.
   */
  void crossfadeTo(const std::string &name, float duration, bool loop = true,
                    float speed = 1.0f);

  /// @brief Pause animation playback (holds the current pose).
  void pauseAnimation();
  /// @brief Resume animation playback.
  void resumeAnimation();

  /// @brief Name of the animation clip currently playing (or blending
  /// from), or an empty string if none is loaded/playing.
  std::string getCurrentAnimation() const;
  /// @brief Whether a crossfade is currently in progress.
  bool isCrossfading() const;
  /// @brief Current blend factor between the previous and new clip, in
  /// [0, 1]; 0 outside of a crossfade.
  float getBlendFactor() const;
  /// @brief Set the animation clip playback rate, in frames per second.
  void setAnimationFPS(float fps);

  /// @brief Number of bones in this model's skeleton.
  int getBoneCount() const;
  /// @brief Name of the bone at `index`.
  std::string getBoneName(int index) const;
  /// @brief Parent bone index of the bone at `index` (-1 if it's a root).
  int getBoneParent(int index) const;

  /// @brief Register a callback invoked at the end of every update().
  void onUpdate(std::function<void(float, Model3D *, SharedManager)>);
  /// @brief Register a callback invoked at the end of every draw().
  void onDraw(std::function<void(Model3D *, SharedManager)>);
};

} // namespace beam
