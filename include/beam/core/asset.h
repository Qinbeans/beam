#pragma once

#include "raylib.h"
#include <memory>
#include <stdexcept>

namespace beam {

// Forward declarations of asset types
class ImageAsset;
class TextureAsset;
class FontAsset;
class SoundAsset;
class MusicAsset;
class MeshAsset;
class MaterialAsset;
class ModelAsset;
class ModelAnimationAsset;

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
  virtual MusicAsset *asMusic() { return nullptr; }
  virtual MeshAsset *asMesh() { return nullptr; }
  virtual MaterialAsset *asMaterial() { return nullptr; }
  virtual ModelAsset *asModel() { return nullptr; }
  virtual ModelAnimationAsset *asModelAnimation() { return nullptr; }
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

// Music, anything longer than ~10 seconds streamed from disk rather than
// fully loaded (see raylib's Music docs); unlike Sound, playback also needs
// UpdateMusicStream() called every frame (see MusicPlayer).
class MusicAsset : public Asset {
public:
  Music data;

  explicit MusicAsset(const Music &m) : data(m) {}
  ~MusicAsset() { UnloadMusicStream(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<MusicAsset>(data);
  }

  MusicAsset *asMusic() override { return this; }
};

// Raw geometry (vertex/index/normal/... buffers), typically produced by
// raylib's GenMesh* functions (see Mesh3D) or as part of a loaded Model.
class MeshAsset : public Asset {
public:
  Mesh data;

  explicit MeshAsset(const Mesh &m) : data(m) {}
  ~MeshAsset() { UnloadMesh(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<MeshAsset>(data);
  }

  MeshAsset *asMesh() override { return this; }
};

// Shader + texture maps applied when drawing a Mesh (see Mesh3D).
class MaterialAsset : public Asset {
public:
  Material data;

  explicit MaterialAsset(const Material &m) : data(m) {}
  ~MaterialAsset() { UnloadMaterial(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<MaterialAsset>(data);
  }

  MaterialAsset *asMaterial() override { return this; }
};

// A loaded 3D model (meshes + materials + optional skeleton), typically
// produced by raylib's LoadModel. UnloadModel() also unloads the meshes and
// materials the model owns, so ModelAsset does not separately wrap them.
class ModelAsset : public Asset {
public:
  Model data;

  explicit ModelAsset(const Model &m) : data(m) {}
  ~ModelAsset() { UnloadModel(data); }

  std::unique_ptr<Asset> clone() const override {
    return std::make_unique<ModelAsset>(data);
  }

  ModelAsset *asModel() override { return this; }
};

// A model's animation clips (e.g. loaded alongside a skeletal Model),
// produced by raylib's LoadModelAnimations. Owns the whole returned array;
// unloaded as a group via UnloadModelAnimations.
class ModelAnimationAsset : public Asset {
public:
  ModelAnimation *data;
  int count;

  ModelAnimationAsset(ModelAnimation *animations, int count)
      : data(animations), count(count) {}
  ~ModelAnimationAsset() { UnloadModelAnimations(data, count); }

  std::unique_ptr<Asset> clone() const override {
    throw std::runtime_error("ModelAnimationAsset cannot be cloned");
  }

  ModelAnimationAsset *asModelAnimation() override { return this; }
};

} // namespace beam