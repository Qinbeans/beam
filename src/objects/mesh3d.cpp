#include "beam/objects/mesh3d.h"

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

#include "rlgl.h"

namespace beam {

Mesh3D::Mesh3D(SharedManager manager, const std::string &name,
               const std::string &cacheKey, Mesh generatedMesh,
               Vector3 position, Vector3 rotationAxis, float rotationAngle,
               Vector3 scale, Color tint, bool wireframe)
    : Object3D(name, position, rotationAxis, rotationAngle, scale, tint),
      wireframe(wireframe) {
  std::string meshKey = cacheKey + "Mesh";
  std::string materialKey = cacheKey + "Material";

  if (!manager->hasAsset(meshKey)) {
    manager->setAsset<Mesh>(meshKey, generatedMesh);
  } else {
    UnloadMesh(generatedMesh);
  }
  mesh = manager->getAsset<Mesh>(meshKey);

  if (!manager->hasAsset(materialKey)) {
    manager->setAsset<Material>(materialKey, LoadMaterialDefault());
  }
  material = manager->getAsset<Material>(materialKey);
  material.maps[MATERIAL_MAP_DIFFUSE].color = tint;
}

void Mesh3D::draw(SharedManager manager) {
  GameObject::draw(manager);
  // Camera3D draws its children unconditionally, so without this setActive
  // (false) would set the flag and change nothing on screen.
  if (!isActive())
    return;

  material.maps[MATERIAL_MAP_DIFFUSE].color = getTint();

  Matrix transform = getWorldMatrix();

  if (wireframe) {
    rlEnableWireMode();
  }
  DrawMesh(mesh, material, transform);
  if (wireframe) {
    rlDisableWireMode();
  }

  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Mesh3D::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Mesh3D::setMesh(SharedManager manager, const std::string &cacheKey,
                     Mesh newMesh) {
  std::string meshKey = cacheKey + "Mesh";
  // Assigning over the entry destroys the asset it held, which unloads the old
  // mesh's GPU buffers; `mesh` is repointed immediately after, so it never
  // refers to the freed one.
  manager->setAsset<Mesh>(meshKey, newMesh);
  mesh = manager->getAsset<Mesh>(meshKey);
}

void Mesh3D::setWireframe(bool wireframe) { this->wireframe = wireframe; }

bool Mesh3D::isWireframe() const { return wireframe; }

BoundingBox Mesh3D::getBoundingBox() const {
  return transformBoundingBox(GetMeshBoundingBox(mesh));
}

void Mesh3D::onUpdate(
    std::function<void(float, Mesh3D *, SharedManager)> callback) {
  updateCallback = callback;
}

void Mesh3D::onDraw(std::function<void(Mesh3D *, SharedManager)> callback) {
  drawCallback = callback;
}

} // namespace beam
