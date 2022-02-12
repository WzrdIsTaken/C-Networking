// Created by Ben on 02/02/2022

#include "iostream"

#include <Engine/FileIO.hpp>
#include <Engine/Logger.hpp>

#include "../../../../include/ASGEGameLib/Components/TileMap/TileMap.hpp"

// --- The actual tilemap ---

bcw::TileMap::TileMap( // NOLINT It does initialise it! Just through a function...
  const LoadMode load_mode, const std::string& file_path, const float _tile_size, // NOLINT >:)
  const bcw::Vector2& _map_offset) :
  map_offset(_map_offset),
  tile_size(_tile_size)
{
  switch (load_mode)
  {
    case LoadMode::FROM_STRING:
      map = file_path;
      break;

    case LoadMode::FROM_FILE:
      ASGE::FILEIO::File tile_map_file;
      if (tile_map_file.open(file_path))
      {
        ASGE::FILEIO::IOBuffer buffer = tile_map_file.read();
        std::string file_string(buffer.as_char(), buffer.length);

        map = file_string; // If we need a [w i d e] string: https://bit.ly/3upern8

        tile_map_file.close();
        return;
      }

      Logging::ERRORS("Something has gone very wrong loading the tilemap");
      break;
  }

  initializeMapDimensions();
  initializeTiles();
}

std::vector<std::unique_ptr<ASGE::Sprite>>
bcw::TileMap::drawMap(const std::map<TileData::TileType, std::string>& sprites, float _tile_scale)
{
  tile_scale = _tile_scale;

  bcw::Vector2Int tile_position = bcw::Vector2Int(0, 0);
  std::vector<std::unique_ptr<ASGE::Sprite>> tile_map_sprites;

  for (char i : map)
  {
    // Reached the end of a row
    if (i == '\n')
    {
      tile_position.x = 0;
      tile_position.y++;
    }
    else
    {
      if (TILE_SYMBOLS.contains(i))
      {
        TileData::TileType tile_type = TILE_SYMBOLS.at(i).tile_type;
        auto& tile_sprite =
          tile_map_sprites.emplace_back(bcw::Driver::getRenderer()->createUniqueSprite());

        bcw::Vector2 tile_pos = bcw::Vector2(
          map_offset.x + static_cast<float>(tile_position.x) * tile_size,
          map_offset.y + static_cast<float>(tile_position.y) * tile_size);
        bcw::Driver::getRenderHandler()->setupSprite(
          tile_sprite.get(), sprites.at(tile_type), tile_pos, tile_scale);
      }

      tile_position.x++;
    }
  }

  return tile_map_sprites;
}

bcw::Vector2Int bcw::TileMap::worldToTile(const bcw::Vector2& world_space) const
{
  return { static_cast<int>(world_space.x / tile_size),
           static_cast<int>(world_space.y / tile_size) };
}

bcw::Vector2 bcw::TileMap::tileToWorld(const bcw::Vector2Int& tile_space) const
{
  return {
    static_cast<float>(
      ((static_cast<float>(tile_space.x + 1)) * tile_size + map_offset.x) - (tile_size * 0.5)),
    static_cast<float>(
      ((static_cast<float>(tile_space.y + 1)) * tile_size + map_offset.y) - (tile_size * 0.5))
  };
}

std::string bcw::TileMap::getMapString() const
{
  return map;
}

std::vector<std::vector<bcw::TileMap::TileData>> bcw::TileMap::getTileData() const
{
  return tiles;
}

bcw::Vector2Int bcw::TileMap::getMapSize() const
{
  return map_dimensions;
}

float bcw::TileMap::getTileSize() const
{
  return tile_size;
}

float bcw::TileMap::getTileScale() const
{
  return tile_scale;
}

bcw::Vector2 bcw::TileMap::getMapOffset() const
{
  return map_offset;
}

void bcw::TileMap::initializeMapDimensions()
{
  // Must be a better way to get the width...
  int map_width = 0;
  for (char i : map)
  {
    if (i != '\n')
    {
      map_width++;
    }
    else
    {
      break;
    }
  }

  long long map_height = std::count(map.begin(), map.end(), '\n') + 1;
  map_dimensions       = bcw::Vector2Int(map_width, static_cast<int>(map_height));
}

void bcw::TileMap::initializeTiles()
{
  bcw::Vector2Int tile_position = bcw::Vector2Int(0, 0);
  std::vector<TileData> tile_row_data;

  for (char& i : map)
  {
    // Reached the end of a row
    if (i == '\n')
    {
      tiles.push_back(tile_row_data);
      tile_row_data.clear();

      tile_position.x = 0;
      tile_position.y++;
    }
    // Iterating through a line
    else if (TILE_SYMBOLS.contains(i))
    {
      TileData tile_data{};
      tile_data.cords = tile_position;

      TileTypeAndCost tile_type_and_cost = TILE_SYMBOLS.at(i);
      tile_data.tile_type                = tile_type_and_cost.tile_type;
      tile_data.cost                     = tile_type_and_cost.tile_cost;

      tile_position.x++;
      tile_row_data.emplace_back(tile_data);
    }
    // Who made the map file?!
    else
    {
      // For some reason something is often not recognised?? ascii value 13 - D?
      // Logging::INFO("Warning! Map character - " + std::to_string(i) + " - is not recognised!");
      tile_position.x++;
    }
  }
}

void bcw::TileMap::printMap() const
{
  // Not using Engine/Logging because it has the [INFO] statement
  std::cout << map << std::endl;
}

// --- Tile Type and Cost ---

bcw::TileMap::TileTypeAndCost::TileTypeAndCost(TileData::TileType _tile_type, int _tile_cost) :
  tile_type(_tile_type), tile_cost(_tile_cost)
{
}
