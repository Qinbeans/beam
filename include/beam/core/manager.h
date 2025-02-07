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

public:
  Manager() = default;

  template <typename T> T &getAsset(const std::string &name) {
    return *static_cast<T *>(assets[name].get());
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
};
} // namespace beam

using SharedManager = std::shared_ptr<beam::Manager>;
