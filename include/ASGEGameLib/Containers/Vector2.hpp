// Created by Ben on 01/02/2022

#pragma once

#include <Engine/Point2D.hpp>

namespace bcw
{
  struct Vector2 : public ASGE::Point2D
  {
   public:
    // Inherit Point2D's constructor
    using ASGE::Point2D::Point2D;
    ~Vector2() = default;

    // Normalizes the vector
    Vector2 normalise();
  };

  struct Vector2Int
  {
   public:
    int x;
    int y;

    Vector2Int() = default;
    Vector2Int(int _x, int _y);
    ~Vector2Int() = default;

    // Work out the distance between two Vector2Ints
    [[maybe_unused]] [[nodiscard]] int distance(const bcw::Vector2Int& other) const;

    // Return the vector as a string
    [[maybe_unused]] [[nodiscard]] std::string toString() const;
  };
} // namespace bcw

// Many vector methods here if needed:
// http://www.technologicalutopia.com/sourcecode/xnageometry/vector2.cs.htm
