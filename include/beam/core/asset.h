#pragma once

#include "raylib.h"

namespace beam {
class Asset {
public:
  union AssetData {
    Image image;
    Texture2D texture;
    Font font;
    Sound sound;

    AssetData() {}
    ~AssetData() {}
  };

private:
  AssetData data;

public:
  template <typename T> T *into() { return reinterpret_cast<T *>(&data); }
};
} // namespace beam