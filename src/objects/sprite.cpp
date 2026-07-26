#include "beam/objects/sprite.h"
#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include "raylib.h"

namespace beam {
Sprite::Sprite(SharedManager manager, const std::string &name, Vector2 position,
               Rectangle bound, Color tint, Color bg)
    : GameObject(name), position(position), bound(bound), tint(tint), bg(bg),
      rotation(0.0), scale(1.0) {
  std::string textureName = name + "Texture";
  if (!manager->hasAsset(textureName)) {
    auto textureMap = manager->getAsset<Image>("textureMap");
    auto imageTexture = ImageFromImage(textureMap, bound);
    ImageClearBackground(&imageTexture, bg);
    auto texture = LoadTextureFromImage(imageTexture);
    manager->setAsset<Texture2D>(textureName, texture);
  }
}

// Declared in the header, so it is Sprite's key function: this definition is
// what makes the compiler emit Sprite's vtable and typeinfo.
Sprite::~Sprite() {}

void Sprite::draw(SharedManager manager) {
  GameObject::draw(manager);
  auto texture = manager->getAsset<Texture2D>(name + "Texture");
  DrawTextureEx(texture, position, rotation, scale, tint);
  if (drawCallback) {
    drawCallback(this, manager);
  }
}

void Sprite::update(float delta, SharedManager manager) {
  GameObject::update(delta, manager);
  if (updateCallback) {
    updateCallback(delta, this, manager);
  }
}

void Sprite::setPosition(Vector2 position) { this->position = position; }

void Sprite::setBound(Rectangle bound) { this->bound = bound; }

void Sprite::setTint(Color tint) { this->tint = tint; }

void Sprite::setBackgroundColor(Color bg) { this->bg = bg; }

void Sprite::setRotation(float rotation) { this->rotation = rotation; }

void Sprite::setScale(float scale) { this->scale = scale; }

Vector2 Sprite::getPosition() const { return position; }

Rectangle Sprite::getBound() const { return bound; }

Color Sprite::getTint() const { return tint; }

Color Sprite::getBackgroundColor() const { return bg; }

float Sprite::getRotation() const { return rotation; }

float Sprite::getScale() const { return scale; }

void Sprite::onUpdate(
    std::function<void(float, Sprite *, SharedManager)> callback) {
  updateCallback = callback;
}

void Sprite::onDraw(std::function<void(Sprite *, SharedManager)> callback) {
  drawCallback = callback;
}
} // namespace beam