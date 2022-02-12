// Created by Ben on 03/02/2022

#pragma once

#include "../../Containers/Vector2.hpp"
#include "TileMap.hpp"

namespace bcw
{
  // If time make a nice pathfinder class.
  // If there isn't time then its ok - this isn't part of the spec.
  // In its current state, this class is not very reusable at all

  class PathFinder
  {
   public:
    explicit PathFinder(bcw::TileMap* tile_map);
    ~PathFinder() = default;

    // Will (soon™) calculates the shortest path between start_pos and end_pos
    [[nodiscard]] std::vector<bcw::Vector2>
    findPath(const bcw::Vector2& start_pos, const bcw::Vector2& end_pos) const;

   private:
  };
} // namespace bcw