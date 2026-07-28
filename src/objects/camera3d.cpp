#include "beam/objects/camera3d.h"

namespace beam {

Camera3D::Camera3D(const std::string &name, Vector3 position, Vector3 target,
                    Vector3 up, float fovy, int projection, int mode)
    : GameObject(name), camera{position, target, up, fovy, projection},
      mode(mode) {}

void Camera3D::init(SharedManager manager) {
  for (auto &child : buffer) {
    child->init(manager);
  }
}

void Camera3D::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (!isActive())
    return;

  if (mode != CAMERA_CUSTOM) {
    UpdateCamera(&camera, mode);
  }

  for (auto &child : buffer) {
    child->update(delta, manager);
  }

  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Camera3D::draw(SharedManager manager) {
  GameObject::draw(manager);
  if (!isActive())
    return;

  BeginMode3D(camera);
  for (auto &child : buffer) {
    child->draw(manager);
  }
  EndMode3D();

  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Camera3D::setPosition(Vector3 position) { camera.position = position; }

void Camera3D::setTarget(Vector3 target) { camera.target = target; }

void Camera3D::setUp(Vector3 up) { camera.up = up; }

void Camera3D::setFovy(float fovy) { camera.fovy = fovy; }

void Camera3D::setProjection(int projection) { camera.projection = projection; }

void Camera3D::setMode(int mode) { this->mode = mode; }

Vector3 Camera3D::getPosition() const { return camera.position; }

Vector3 Camera3D::getTarget() const { return camera.target; }

Vector3 Camera3D::getUp() const { return camera.up; }

float Camera3D::getFovy() const { return camera.fovy; }

int Camera3D::getProjection() const { return camera.projection; }

int Camera3D::getMode() const { return mode; }

Matrix Camera3D::getViewMatrix() const { return GetCameraMatrix(camera); }

Vector2 Camera3D::worldToScreen(Vector3 point) const {
  return GetWorldToScreen(point, camera);
}

Ray Camera3D::screenToWorldRay(Vector2 point) const {
  return GetScreenToWorldRay(point, camera);
}

void Camera3D::onUpdate(
    std::function<void(float, Camera3D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Camera3D::onDraw(
    std::function<void(Camera3D *, SharedManager)> callback) {
  drawCallback = callback;
}

} // namespace beam