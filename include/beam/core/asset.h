#pragma once

#include "raylib.h"
#include <memory>

namespace beam {

// Forward declarations of asset types
class ImageAsset;
class TextureAsset;
class FontAsset;
class SoundAsset;

// Base Asset class
class Asset {
public:
  virtual ~Asset() = default;

  // Virtual clone method for polymorphic copying
  virtual std::unique_ptr<Asset> clone() const = 0;

  // Type-safe conversion methods
  virtual ImageAsset *asImage() { return nullptr; }
  virtual TextureAsset *asTexture() { return nullptr; }
  virtual FontAsset *asFont() { return nullptr; }
  virtual SoundAsset *asSound() { return nullptr; }
};

// Concrete asset types
class ImageAsset : public Asset {
public:
  Image data;

  explicit ImageAsset(const Image &img) : data(img) {}
  ~ImageAsset() { UnloadImage(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<ImageAsset>(data);
  }

  ImageAsset *asImage() override { return this; }
};

class TextureAsset : public Asset {
public:
  Texture2D data;

  explicit TextureAsset(const Texture2D &tex) : data(tex) {}
  ~TextureAsset() { UnloadTexture(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<TextureAsset>(data);
  }

  TextureAsset *asTexture() override { return this; }
};

class FontAsset : public Asset {
public:
  Font data;

  explicit FontAsset(const Font &f) : data(f) {}
  ~FontAsset() { UnloadFont(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<FontAsset>(data);
  }

  FontAsset *asFont() override { return this; }
};

class SoundAsset : public Asset {
public:
  Sound data;

  explicit SoundAsset(const Sound &s) : data(s) {}
  ~SoundAsset() { UnloadSound(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<SoundAsset>(data);
  }

  SoundAsset *asSound() override { return this; }
};

} // namespace beam