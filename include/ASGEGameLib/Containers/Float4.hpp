// Created by Ben on 01/02/2022.

#pragma once

namespace bcw
{
  struct Float4
  {
   public:
    float x{};
    float y{};
    float width{};
    float height{};

    Float4(float _x, float _y, float _width, float _height);
    ~Float4() = default;
  };
} // namespace bcw
