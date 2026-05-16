#pragma once

#include "beam/core/manager.h"
#include "beam/objects/game_object.h"
#include <functional>
#include <raylib.h>

namespace beam {

// Type alias for the randomizer function
// Parameters: (tileX, tileY, worldX, worldY) -> tileIndex
using TileRandomizerFunc = std::function<int(int, int, float, float)>;

class TileMap : public GameObject {
private:
  std::vector<std::vector<int>> tiles; // 2D vector to hold tile indices
  Texture2D baseTexture;               // Base image texture for tiles
  int tileWidth;                       // Width of each tile in pixels
  int tileHeight;                      // Height of each tile in pixels
  int mapWidth;                        // Width of the tilemap in tiles
  int mapHeight;                       // Height of the tilemap in tiles
  float worldX;                        // World X position of the tilemap
  float worldY;                        // World Y position of the tilemap
  Color tint;                          // Tint color for rendering
  TileRandomizerFunc randomizerFunc;   // Function to generate tile indices
  int tilesPerRow;    // Number of tiles per row in the base texture
  int tilesPerColumn; // Number of tiles per column in the base texture
  int totalTileCount; // Total number of tiles in the texture
  bool textureLoaded; // Flag to track if texture is loaded

  // Calculate tile counts from texture dimensions
  void calculateTileCounts();

public:
  // Constructor for pre-defined tile layout
  TileMap(const std::string &name, const std::vector<std::vector<int>> &tiles,
          int tileWidth, int tileHeight, Color tint = WHITE);

  // Constructor for procedurally generated tilemap
  TileMap(const std::string &name, const std::string &texturePath,
          int tileWidth, int tileHeight, int mapWidth, int mapHeight,
          float worldX, float worldY, TileRandomizerFunc randomizer,
          Color tint = WHITE);

  ~TileMap();

  // Generate the tilemap using the randomizer function
  void generateTiles();

  // Load texture from file path
  bool loadTexture(const std::string &texturePath);

  // Set the randomizer function
  void setRandomizerFunction(TileRandomizerFunc randomizer);

  // Set the base texture and calculate tiles per row/column
  void setBaseTexture(const Texture2D &texture);

  // Get tile index at specific tile coordinates
  int getTileAt(int tileX, int tileY) const;

  // Set tile index at specific tile coordinates
  void setTileAt(int tileX, int tileY, int tileIndex);

  // Get world position
  Vector2 getWorldPosition() const;

  // Set world position
  void setWorldPosition(float x, float y);

  // Get map dimensions
  Vector2 getMapSize() const;

  // Get texture tile dimensions (tiles per row/column)
  Vector2 getTextureTileCount() const;

  // Get total number of tiles in texture
  int getTotalTileCount() const;

  // Regenerate a specific area of the tilemap
  void regenerateArea(int startX, int startY, int width, int height);

  void draw(SharedManager) override;
  void update(float, SharedManager) override;
}; // TileMap

} // namespace beam