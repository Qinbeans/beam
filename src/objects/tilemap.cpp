#include "beam/objects/tilemap.h"
#include <algorithm>
#include <raylib.h>
#include <string>
#include <vector>

namespace beam {

// Constructor for pre-defined tile layout
TileMap::TileMap(const std::string &name,
                 const std::vector<std::vector<int>> &tiles, int tileWidth,
                 int tileHeight, Color tint)
    : GameObject(name), tiles(tiles), tileWidth(tileWidth),
      tileHeight(tileHeight), mapWidth(tiles.empty() ? 0 : tiles[0].size()),
      mapHeight(tiles.size()), worldX(0.0f), worldY(0.0f), tint(tint),
      tilesPerRow(1), tilesPerColumn(1), totalTileCount(1),
      textureLoaded(false) {
  // Initialize empty texture
  baseTexture = {0, 0, 0, 0, 0};
}

// Constructor for procedurally generated tilemap
TileMap::TileMap(const std::string &name, const std::string &texturePath,
                 int tileWidth, int tileHeight, int mapWidth, int mapHeight,
                 float worldX, float worldY, TileRandomizerFunc randomizer,
                 Color tint)
    : GameObject(name), tileWidth(tileWidth), tileHeight(tileHeight),
      mapWidth(mapWidth), mapHeight(mapHeight), worldX(worldX), worldY(worldY),
      tint(tint), randomizerFunc(randomizer), tilesPerRow(1), tilesPerColumn(1),
      totalTileCount(1), textureLoaded(false) {

  // Initialize tiles vector
  tiles.resize(mapHeight, std::vector<int>(mapWidth, 0));

  // Load texture
  if (!texturePath.empty()) {
    loadTexture(texturePath);
  } else {
    baseTexture = {0, 0, 0, 0, 0};
  }

  // Generate tiles if randomizer is provided
  if (randomizerFunc) {
    generateTiles();
  }
}
TileMap::~TileMap() {
  if (textureLoaded && baseTexture.id != 0) {
    UnloadTexture(baseTexture);
  }
}

void TileMap::calculateTileCounts() {
  if (!textureLoaded || baseTexture.id == 0 || tileWidth <= 0 ||
      tileHeight <= 0) {
    tilesPerRow = 1;
    tilesPerColumn = 1;
    totalTileCount = 1;
    return;
  }

  // Calculate number of tiles using math instead of iteration
  tilesPerRow = baseTexture.width / tileWidth;
  tilesPerColumn = baseTexture.height / tileHeight;
  totalTileCount = tilesPerRow * tilesPerColumn;

  // Ensure we have at least 1 tile to avoid division by zero
  if (tilesPerRow <= 0)
    tilesPerRow = 1;
  if (tilesPerColumn <= 0)
    tilesPerColumn = 1;
  if (totalTileCount <= 0)
    totalTileCount = 1;
}

void TileMap::generateTiles() {
  if (!randomizerFunc)
    return;

  for (int y = 0; y < mapHeight; ++y) {
    for (int x = 0; x < mapWidth; ++x) {
      // Calculate world coordinates for this tile
      float currentWorldX = worldX + (x * tileWidth);
      float currentWorldY = worldY + (y * tileHeight);

      // Use the randomizer function to determine tile index
      tiles[y][x] = randomizerFunc(x, y, currentWorldX, currentWorldY);
    }
  }
}

bool TileMap::loadTexture(const std::string &texturePath) {
  if (textureLoaded && baseTexture.id != 0) {
    UnloadTexture(baseTexture);
  }

  baseTexture = LoadTexture(texturePath.c_str());
  textureLoaded = (baseTexture.id != 0);

  if (textureLoaded) {
    // Calculate tile counts using math
    calculateTileCounts();
  }

  return textureLoaded;
}
void TileMap::setRandomizerFunction(TileRandomizerFunc randomizer) {
  randomizerFunc = randomizer;
}

void TileMap::setBaseTexture(const Texture2D &texture) {
  if (textureLoaded && baseTexture.id != 0) {
    UnloadTexture(baseTexture);
  }

  baseTexture = texture;
  textureLoaded = true;
  calculateTileCounts();
}
int TileMap::getTileAt(int tileX, int tileY) const {
  if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
    return tiles[tileY][tileX];
  }
  return -1; // Invalid tile
}

void TileMap::setTileAt(int tileX, int tileY, int tileIndex) {
  if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
    tiles[tileY][tileX] = tileIndex;
  }
}

Vector2 TileMap::getWorldPosition() const { return {worldX, worldY}; }

void TileMap::setWorldPosition(float x, float y) {
  worldX = x;
  worldY = y;
  // Update the Node's position as well
  setPosition(x, y);
}

Vector2 TileMap::getMapSize() const {
  return {static_cast<float>(mapWidth), static_cast<float>(mapHeight)};
}

Vector2 TileMap::getTextureTileCount() const {
  return {static_cast<float>(tilesPerRow), static_cast<float>(tilesPerColumn)};
}

int TileMap::getTotalTileCount() const { return totalTileCount; }

void TileMap::regenerateArea(int startX, int startY, int width, int height) {
  if (!randomizerFunc)
    return;

  int endX = std::min(startX + width, mapWidth);
  int endY = std::min(startY + height, mapHeight);

  for (int y = startY; y < endY; ++y) {
    for (int x = startX; x < endX; ++x) {
      if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        float currentWorldX = worldX + (x * tileWidth);
        float currentWorldY = worldY + (y * tileHeight);
        tiles[y][x] = randomizerFunc(x, y, currentWorldX, currentWorldY);
      }
    }
  }
}

void TileMap::draw(SharedManager) {
  if (!isActive() || !textureLoaded || baseTexture.id == 0)
    return;

  Vector2 nodePos = getPosition();
  float renderX = nodePos.x + worldX;
  float renderY = nodePos.y + worldY;

  for (int y = 0; y < mapHeight; ++y) {
    for (int x = 0; x < mapWidth; ++x) {
      int tileIndex = tiles[y][x];
      if (tileIndex < 0)
        continue; // Skip invalid tiles

      // Calculate source rectangle in the texture
      int sourceX = (tileIndex % tilesPerRow) * tileWidth;
      int sourceY = (tileIndex / tilesPerRow) * tileHeight;

      Rectangle sourceRect = {
          static_cast<float>(sourceX), static_cast<float>(sourceY),
          static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

      // Calculate destination rectangle
      Rectangle destRect = {
          renderX + (x * tileWidth), renderY + (y * tileHeight),
          static_cast<float>(tileWidth), static_cast<float>(tileHeight)};

      // Draw the tile
      DrawTexturePro(baseTexture, sourceRect, destRect, {0, 0}, 0.0f, tint);
    }
  }
}

void TileMap::update(float deltaTime, SharedManager manager) {
  GameObject::update(deltaTime, manager);
  // Add any tilemap-specific update logic here
}

} // namespace beam
