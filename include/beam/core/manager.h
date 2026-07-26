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

  // Helper template to map raylib types to asset types
  template <typename T> struct AssetTypeMap {
    using type = void; // Default case
  };

public:
  Manager() : bg(RAYWHITE), isclosed(false) {}

  // Manager owns Assets via unique_ptr and is always shared through
  // SharedManager; deleting these explicitly (rather than leaving them
  // implicitly deleted) keeps compilers from having to instantiate the
  // asset map's copy constructor just to determine copyability.
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

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
      assets[name] = std::make_unique<SoundAsset>(asset);
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