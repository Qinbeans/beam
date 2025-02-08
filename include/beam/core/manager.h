#pragma once

#include "beam/core/asset.h"
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

using AssetManager =
    std::unordered_map<std::string, std::unique_ptr<beam::Asset>>;
using StateManager = std::unordered_map<std::string, std::any>;

namespace beam {

class Manager {
private:
  AssetManager assets;
  StateManager states;
  Color bg;
  bool isclosed = false;

public:
  Manager() {
    assets = AssetManager();
    states = StateManager();
    bg = RAYWHITE;
    isclosed = false;
  };

  bool hasAsset(const std::string &name) {
    if (assets.empty()) {
      return false;
    }
    return assets.find(name) != assets.end();
  }

  template <typename T> T &getAsset(const std::string &name) {
    return *assets[name]->into<T>();
  }

  template <typename T> void setAsset(const std::string &name, T asset) {
    assets[name] = std::make_unique<T>(asset);
  }

  template <typename T> T &getState(const std::string &name) {
    return std::any_cast<T &>(states[name]);
  }

  template <typename T> void setState(const std::string &name, T value) {
    states[name] = value;
  }

  void close() { isclosed = true; }

  bool closed() const { return isclosed; }

  void setBackgroundColor(Color bg) { this->bg = bg; }

  Color getBackgroundColor() const { return bg; }

  int getKeyPressed() const { return GetKeyPressed(); }

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
};
} // namespace beam

using SharedManager = std::shared_ptr<beam::Manager>;
