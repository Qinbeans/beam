#include "beam/objects/model3d.h"
#include <algorithm>
#include <cmath>
#include <limits>

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

namespace beam {

Model3D::Model3D(SharedManager manager, const std::string &name,
                  const std::string &modelPath, Vector3 position,
                  Vector3 rotationAxis, float rotationAngle, Vector3 scale,
                  Color tint, bool wireframe)
    : Object3D(name, position, rotationAxis, rotationAngle, scale, tint),
      wireframe(wireframe), animations(nullptr), animationCount(0),
      fromIndex(-1), toIndex(-1), frameA(0.0f), frameB(0.0f), blend(0.0f),
      crossfading(false), blendDuration(0.0f), blendElapsed(0.0f),
      loop(true), speed(1.0f), fps(30.0f), playing(false) {
  std::string assetKey = modelPath + "Model";
  if (!manager->hasAsset(assetKey)) {
    manager->setAsset<Model>(assetKey, LoadModel(modelPath.c_str()));
  }
  model = manager->getAsset<Model>(assetKey);
}

void Model3D::draw(SharedManager manager) {
  GameObject::draw(manager);

  if (wireframe) {
    DrawModelWiresEx(model, getPosition(), getRotationAxis(),
                      getRotationAngle(), getScale(), getTint());
  } else {
    DrawModelEx(model, getPosition(), getRotationAxis(), getRotationAngle(),
                getScale(), getTint());
  }

  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Model3D::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);

  if (playing && animations != nullptr && fromIndex >= 0) {
    const ModelAnimation &animA = animations[fromIndex];
    frameA += delta * fps * speed;
    if (animA.frameCount > 0) {
      if (loop) {
        frameA = std::fmod(frameA, static_cast<float>(animA.frameCount));
        if (frameA < 0.0f)
          frameA += static_cast<float>(animA.frameCount);
      } else {
        frameA = std::min(frameA, static_cast<float>(animA.frameCount - 1));
      }
    }

    if (crossfading && toIndex >= 0) {
      const ModelAnimation &animB = animations[toIndex];
      frameB += delta * fps * speed;
      if (animB.frameCount > 0) {
        frameB = std::fmod(frameB, static_cast<float>(animB.frameCount));
        if (frameB < 0.0f)
          frameB += static_cast<float>(animB.frameCount);
      }

      blendElapsed += delta;
      blend = blendDuration > 0.0f
                  ? std::min(blendElapsed / blendDuration, 1.0f)
                  : 1.0f;

      UpdateModelAnimationEx(model, animA, frameA, animB, frameB, blend);

      if (blend >= 1.0f) {
        fromIndex = toIndex;
        frameA = frameB;
        toIndex = -1;
        blend = 0.0f;
        crossfading = false;
      }
    } else {
      UpdateModelAnimationEx(model, animA, frameA, animA, frameA, 0.0f);
    }
  }

  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Model3D::setWireframe(bool wireframe) { this->wireframe = wireframe; }

bool Model3D::isWireframe() const { return wireframe; }

BoundingBox Model3D::getBoundingBox() const {
  BoundingBox box = GetModelBoundingBox(model);
  Vector3 position = getPosition();
  box.min.x += position.x;
  box.min.y += position.y;
  box.min.z += position.z;
  box.max.x += position.x;
  box.max.y += position.y;
  box.max.z += position.z;
  return box;
}

RayCollision Model3D::checkRayCollision(Ray ray) const {
  RayCollision closest{false, std::numeric_limits<float>::max(),
                       Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f}};

  Matrix transform = MatrixMultiply(
      model.transform,
      MatrixMultiply(
          MatrixMultiply(MatrixScale(getScale().x, getScale().y, getScale().z),
                          MatrixRotate(getRotationAxis(),
                                       getRotationAngle() * DEG2RAD)),
          MatrixTranslate(getPosition().x, getPosition().y,
                           getPosition().z)));

  for (int i = 0; i < model.meshCount; i++) {
    RayCollision collision =
        GetRayCollisionMesh(ray, model.meshes[i], transform);
    if (collision.hit && collision.distance < closest.distance) {
      closest = collision;
    }
  }

  if (!closest.hit) {
    closest.distance = 0.0f;
  }

  return closest;
}

void Model3D::onUpdate(
    std::function<void(float, Model3D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Model3D::onDraw(std::function<void(Model3D *, SharedManager)> callback) {
  drawCallback = callback;
}

void Model3D::loadAnimations(SharedManager manager,
                              const std::string &animationPath) {
  std::string assetKey = animationPath + "Animations";
  if (!manager->hasAsset(assetKey)) {
    int count = 0;
    ModelAnimation *loaded =
        LoadModelAnimations(animationPath.c_str(), &count);
    manager->setAnimations(assetKey, loaded, count);
  }
  animations = manager->getAnimations(assetKey, animationCount);
}

std::vector<std::string> Model3D::getAnimationNames() const {
  std::vector<std::string> names;
  for (int i = 0; i < animationCount; i++) {
    if (IsModelAnimationValid(model, animations[i])) {
      names.emplace_back(animations[i].name);
    }
  }
  return names;
}

static int findAnimationIndex(ModelAnimation *animations, int animationCount,
                               const Model &model, const std::string &name) {
  for (int i = 0; i < animationCount; i++) {
    if (name == animations[i].name && IsModelAnimationValid(model, animations[i])) {
      return i;
    }
  }
  return -1;
}

void Model3D::playAnimation(const std::string &name, bool loop, float speed) {
  int index = findAnimationIndex(animations, animationCount, model, name);
  if (index < 0)
    return;

  fromIndex = index;
  toIndex = -1;
  frameA = 0.0f;
  frameB = 0.0f;
  blend = 0.0f;
  crossfading = false;
  this->loop = loop;
  this->speed = speed;
  playing = true;
}

void Model3D::crossfadeTo(const std::string &name, float duration, bool loop,
                           float speed) {
  int index = findAnimationIndex(animations, animationCount, model, name);
  if (index < 0)
    return;

  if (fromIndex < 0) {
    playAnimation(name, loop, speed);
    return;
  }

  toIndex = index;
  frameB = 0.0f;
  blend = 0.0f;
  blendDuration = duration;
  blendElapsed = 0.0f;
  crossfading = true;
  this->loop = loop;
  this->speed = speed;
  playing = true;
}

void Model3D::pauseAnimation() { playing = false; }

void Model3D::resumeAnimation() { playing = true; }

std::string Model3D::getCurrentAnimation() const {
  if (fromIndex < 0 || animations == nullptr)
    return "";
  return animations[fromIndex].name;
}

bool Model3D::isCrossfading() const { return crossfading; }

float Model3D::getBlendFactor() const { return blend; }

void Model3D::setAnimationFPS(float fps) { this->fps = fps; }

int Model3D::getBoneCount() const { return model.boneCount; }

std::string Model3D::getBoneName(int index) const {
  if (index < 0 || index >= model.boneCount)
    return "";
  return model.bones[index].name;
}

int Model3D::getBoneParent(int index) const {
  if (index < 0 || index >= model.boneCount)
    return -1;
  return model.bones[index].parent;
}

} // namespace beam
