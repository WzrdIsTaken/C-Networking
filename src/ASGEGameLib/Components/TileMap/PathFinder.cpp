// Created by Ben on 03/02/2022

#include "../../../../include/ASGEGameLib/Components/TileMap/PathFinder.hpp"

bcw::PathFinder::PathFinder(bcw::TileMap* /*tile_map*/) {}

std::vector<bcw::Vector2>
bcw::PathFinder::findPath(const bcw::Vector2& /*start_pos*/, const bcw::Vector2& end_pos) const
{
  // Step one - create the path vector
  std::vector<bcw::Vector2> path;

  // Step two - ???

  // Step three - profit
  path.push_back(end_pos);
  return path;
}
