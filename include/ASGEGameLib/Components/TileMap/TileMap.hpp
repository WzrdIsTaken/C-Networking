// Created by Ben on 02/02/2022

#pragma once

#include "../../Containers/Vector2.hpp"
#include "../../Driver.hpp"

// Note: This tile map works with text files because they are just easier

namespace bcw
{
  class TileMap // NOLINT dont care
  {
   public:
    struct TileData
    {
      // The name of the tile
      std::string name = "https://youtu.be/1Zn958rN-H4";

      // The coordinates of the tile
      bcw::Vector2Int cords = bcw::Vector2Int(0, 0);

      // The cost of the tile
      int cost = 0;

      enum TileType
      {
        WALL,
        FLOOR
      };
      TileType tile_type;
    };

    enum LoadMode
    {
      FROM_FILE,
      FROM_STRING
    };
    [[maybe_unused]] TileMap(
      LoadMode load_mode, const std::string& file_path, float _tile_size,
      const bcw::Vector2& _map_offset = bcw::Vector2(0, 0));
    ~TileMap() = default;

    // Draws the map based off the supplied images
    [[maybe_unused]] std::vector<std::unique_ptr<ASGE::Sprite>>
    drawMap(const std::map<TileData::TileType, std::string>& sprites, float _tile_scale);

    // Converts a world position to tile coordinates
    [[nodiscard]] bcw::Vector2Int worldToTile(const bcw::Vector2& world_space) const;
    // Convert tile coordinates to world position
    [[nodiscard]] bcw::Vector2 tileToWorld(const bcw::Vector2Int& tile_space) const;

    // Returns the map string
    [[maybe_unused]] [[nodiscard]] std::string getMapString() const;
    // Returns tiles
    [[maybe_unused]] [[nodiscard]] std::vector<std::vector<TileData>> getTileData() const;
    // Returns the map's dimensions
    [[maybe_unused]] [[nodiscard]] bcw::Vector2Int getMapSize() const;
    // Returns tile_size
    [[maybe_unused]] [[nodiscard]] float getTileSize() const;
    // Returns tile_scale
    [[maybe_unused]] [[nodiscard]] float getTileScale() const;
    // Returns map_offset
    [[maybe_unused]] [[nodiscard]] bcw::Vector2 getMapOffset() const;

    // Print the map
    [[maybe_unused]] void printMap() const;

   private:
    struct TileTypeAndCost
    {
      TileData::TileType tile_type;
      int tile_cost;

      TileTypeAndCost(TileData::TileType _tile_type, int _tile_cost);
    };

    // Create the TileData structs for the map
    void initializeTiles();
    // Works out the x/y of the map
    void initializeMapDimensions();

    // The file string
    std::string map;

    // All the tile data
    std::vector<std::vector<TileData>> tiles;

    // The maps size
    bcw::Vector2Int map_dimensions;

    // The offset of the tilemap, eg if the tilemap is smaller than the screen
    // and wants to be drawn in the middle
    bcw::Vector2 map_offset;

    // How big the tiles should be
    float tile_size;
    // What the scale of the drawn sprites is (note - not initialized until drawMap is called)
    float tile_scale = 0;

    // Map symbols
    const std::map<char, TileTypeAndCost> TILE_SYMBOLS = {
      { '#', TileTypeAndCost(TileData::TileType::WALL, 1000) },
      { '.', TileTypeAndCost(TileData::TileType::FLOOR, 1) }
    };
  };
} // namespace bcw