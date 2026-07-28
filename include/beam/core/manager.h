#pragma once

#include "beam/core/asset.h"
#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace beam {

class Manager {
private:
  std::unordered_map<std::string, std::unique_ptr<Asset>> assets;
  std::unordered_map<std::string, std::any> states;
  Color bg;
  bool isclosed;
  bool audioDeviceReady;

  // Lazily starts raylib's audio device the first time a Sound/Music asset
  // is stored, so apps that never use audio never pay InitAudioDevice's
  // cost. Safe to call more than once; only the first call does anything.
  void ensureAudioDevice() {
    if (!audioDeviceReady) {
      InitAudioDevice();
      audioDeviceReady = true;
    }
  }

  // Helper template to map raylib types to asset types
  template <typename T> struct AssetTypeMap {
    using type = void; // Default case
  };

public:
  Manager() : bg(RAYWHITE), isclosed(false), audioDeviceReady(false) {}

  // Manager owns Assets via unique_ptr and is always shared through
  // SharedManager; deleting these explicitly (rather than leaving them
  // implicitly deleted) keeps compilers from having to instantiate the
  // asset map's copy constructor just to determine copyability.
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  // Shuts down the audio device if ensureAudioDevice() ever started it.
  // A destructor body runs *before* member destruction, so `assets` (which
  // may own Sound/Music resources whose dtors call UnloadSound/
  // UnloadMusicStream) is cleared explicitly here first, ensuring every
  // audio asset is unloaded while the device is still open.
  ~Manager() {
    assets.clear();
    if (audioDeviceReady) {
      CloseAudioDevice();
    }
  }

  bool hasAsset(const std::string &name) {
    return !assets.empty() && assets.find(name) != assets.end();
  }

  template <typename T> T &getAsset(const std::string &name) {
    auto it = assets.find(name);
    if (it == assets.end()) {
      throw std::runtime_error("Asset not found: " + name);
    }

    if constexpr (std::is_same_v<T, Image>) {
      auto *asset = it->second->asImage();
      if (!asset)
        throw std::runtime_error("Asset is not an Image");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Texture2D>) {
      auto *asset = it->second->asTexture();
      if (!asset)
        throw std::runtime_error("Asset is not a Texture2D");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Font>) {
      auto *asset = it->second->asFont();
      if (!asset)
        throw std::runtime_error("Asset is not a Font");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Sound>) {
      auto *asset = it->second->asSound();
      if (!asset)
        throw std::runtime_error("Asset is not a Sound");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Music>) {
      auto *asset = it->second->asMusic();
      if (!asset)
        throw std::runtime_error("Asset is not a Music");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Mesh>) {
      auto *asset = it->second->asMesh();
      if (!asset)
        throw std::runtime_error("Asset is not a Mesh");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Material>) {
      auto *asset = it->second->asMaterial();
      if (!asset)
        throw std::runtime_error("Asset is not a Material");
      return asset->data;
    } else if constexpr (std::is_same_v<T, Model>) {
      auto *asset = it->second->asModel();
      if (!asset)
        throw std::runtime_error("Asset is not a Model");
      return asset->data;
    } else {
      static_assert(always_false<T>::value, "Unsupported asset type");
    }
  }

  template <typename T> void setAsset(const std::string &name, T asset) {
    if constexpr (std::is_same_v<T, Image>) {
      assets[name] = std::make_unique<ImageAsset>(asset);
    } else if constexpr (std::is_same_v<T, Texture2D>) {
      assets[name] = std::make_unique<TextureAsset>(asset);
    } else if constexpr (std::is_same_v<T, Font>) {
      assets[name] = std::make_unique<FontAsset>(asset);
    } else if constexpr (std::is_same_v<T, Sound>) {
      ensureAudioDevice();
      assets[name] = std::make_unique<SoundAsset>(asset);
    } else if constexpr (std::is_same_v<T, Music>) {
      ensureAudioDevice();
      assets[name] = std::make_unique<MusicAsset>(asset);
    } else if constexpr (std::is_same_v<T, Mesh>) {
      assets[name] = std::make_unique<MeshAsset>(asset);
    } else if constexpr (std::is_same_v<T, Material>) {
      assets[name] = std::make_unique<MaterialAsset>(asset);
    } else if constexpr (std::is_same_v<T, Model>) {
      assets[name] = std::make_unique<ModelAsset>(asset);
    } else {
      static_assert(always_false<T>::value, "Unsupported asset type");
    }
  }

  template <typename T> T &getState(const std::string &name) {
    return std::any_cast<T &>(states[name]);
  }

  template <typename T> void setState(const std::string &name, T value) {
    states[name] = value;
  }

  // Model animation clips are a pointer+count pair (raylib's
  // LoadModelAnimations returns a heap array), not a single copyable
  // raylib value struct, so they don't fit the getAsset<T>/setAsset<T>
  // pattern used above and get their own accessors instead.
  void setAnimations(const std::string &name, ModelAnimation *animations,
                      int count) {
    assets[name] = std::make_unique<ModelAnimationAsset>(animations, count);
  }

  ModelAnimation *getAnimations(const std::string &name, int &count) {
    auto it = assets.find(name);
    if (it == assets.end()) {
      throw std::runtime_error("Asset not found: " + name);
    }
    auto *asset = it->second->asModelAnimation();
    if (!asset)
      throw std::runtime_error("Asset is not a ModelAnimation set");
    count = asset->count;
    return asset->data;
  }

  void close() { isclosed = true; }
  bool closed() const { return isclosed; }

  void setBackgroundColor(Color color) { bg = color; }
  Color getBackgroundColor() const { return bg; }

  int getScreenWidth() const { return GetScreenWidth(); }
  int getScreenHeight() const { return GetScreenHeight(); }
  int getWindowWidth() const { return GetRenderWidth(); }
  int getWindowHeight() const { return GetRenderHeight(); }
  int getFPS() const { return GetFPS(); }
  bool isWindowResized() const { return IsWindowResized(); }

  // Input handling methods
  int getKeyPressed() const { return GetKeyPressed(); }
  char getCharPressed() const { return GetCharPressed(); }
  int getMouseX() const { return GetMouseX(); }
  int getMouseY() const { return GetMouseY(); }

  bool isMouseButtonDown(int button) const { return IsMouseButtonDown(button); }
  bool isMouseButtonPressed(int button) const {
    return IsMouseButtonPressed(button);
  }
  bool isMouseButtonReleased(int button) const {
    return IsMouseButtonReleased(button);
  }

  bool isKeyDown(int key) const { return IsKeyDown(key); }
  bool isKeyPressed(int key) const { return IsKeyPressed(key); }
  bool isKeyReleased(int key) const { return IsKeyReleased(key); }

private:
  template <typename T> struct always_false : std::false_type {};
};

} // namespace beam

using SharedManager = std::shared_ptr<beam::Manager>;