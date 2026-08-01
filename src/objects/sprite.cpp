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

    // bg belongs *behind* the cut, not over it. ImageClearBackground fills the
    // whole image, so calling it here overwrote the region that had just been
    // cut out -- and since bg defaults to BLANK, every sprite ended up a
    // rectangle of fully transparent pixels. It drew; it just drew nothing.
    if (bg.a > 0) {
      Image canvas = GenImageColor(static_cast<int>(bound.width),
                                   static_cast<int>(bound.height), bg);
      Rectangle whole = {0.0f, 0.0f, bound.width, bound.height};
      ImageDraw(&canvas, imageTexture, whole, whole, WHITE);
      UnloadImage(imageTexture);
      imageTexture = canvas;
    }

    auto texture = LoadTextureFromImage(imageTexture);
    // The pixels are on the GPU now, and this copy was ours to free.
    UnloadImage(imageTexture);
    manager->setAsset<Texture2D>(textureName, texture);
  }
}

// Declared in the header, so it is Sprite's key function: this definition is
// what makes the compiler emit Sprite's vtable and typeinfo.
Sprite::~Sprite() {}

void Sprite::draw(SharedManager manager) {
  GameObject::draw(manager);
  // Every other 2D object checks this; Sprite was the one that did not, so
  // setActive(false) set the flag and the sprite carried on drawing.
  if (!active) {
    return;
  }
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