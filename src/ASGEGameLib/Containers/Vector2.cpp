// Created by Ben on 01/02/2022

#include "../../../include/ASGEGameLib/Containers/Vector2.hpp"

#include <cmath>

// --- Vector 2 ---

bcw::Vector2 bcw::Vector2::normalise()
{
  float value = 1.0F / std::sqrt((x * x) + (y * y));
  x *= value;
  y *= value;
  return { x, y };
}

// --- Vector 2 int ---

bcw::Vector2Int::Vector2Int(int _x, int _y) : x(_x), y(_y) {}

int bcw::Vector2Int::distance(const bcw::Vector2Int& other) const
{
  int v1 = x - other.x;
  int v2 = y - other.y;
  return abs(static_cast<int>(sqrt((v1 * v1) + (v2 * v2))));
}

std::string bcw::Vector2Int::toString() const
{
  return std::string("x: " + std::to_string(x) + " y: " + std::to_string(y));
}
