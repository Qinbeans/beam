#include "beam/objects/camera2d.h"

namespace beam {

Camera2D::Camera2D(const std::string &name, Vector2 offset, Vector2 target,
                    float rotation, float zoom)
    : GameObject(name), camera{offset, target, rotation, zoom} {}

void Camera2D::init(SharedManager manager) {
  for (auto &child : buffer) {
    child->init(manager);
  }
}

void Camera2D::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (!isActive())
    return;

  for (auto &child : buffer) {
    child->update(delta, manager);
  }

  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Camera2D::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!isActive())
    return;

  BeginMode2D(camera);
  for (auto &child : buffer) {
    child->draw(manager);
  }
  EndMode2D();

  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Camera2D::setOffset(Vector2 offset) { camera.offset = offset; }

void Camera2D::setTarget(Vector2 target) { camera.target = target; }

void Camera2D::setRotation(float rotation) { camera.rotation = rotation; }

void Camera2D::setZoom(float zoom) { camera.zoom = zoom; }

Vector2 Camera2D::getOffset() const { return camera.offset; }

Vector2 Camera2D::getTarget() const { return camera.target; }

float Camera2D::getRotation() const { return camera.rotation; }

float Camera2D::getZoom() const { return camera.zoom; }

Matrix Camera2D::getViewMatrix() const { return GetCameraMatrix2D(camera); }

Vector2 Camera2D::worldToScreen(Vector2 point) const {
  return GetWorldToScreen2D(point, camera);
}

Vector2 Camera2D::screenToWorld(Vector2 point) const {
  return GetScreenToWorld2D(point, camera);
}

void Camera2D::onUpdate(
    std::function<void(float, Camera2D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Camera2D::onDraw(
    std::function<void(Camera2D *, SharedManager)> callback) {
  drawCallback = callback;
}

} // namespace beam
