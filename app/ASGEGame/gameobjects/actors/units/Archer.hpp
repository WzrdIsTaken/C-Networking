// Created by Ben on 04/02/2022

#pragma once

#include "Unit.hpp"

class Archer : public Unit
{
 public:
  using Unit::Unit;
  ~Archer() override = default;

  // but if we make it virtual then that's redundant because its already overridden...
  void start() override;
};
