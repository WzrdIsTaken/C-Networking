// Created by Ben on 04/02/2022

#pragma once

#include "Unit.hpp"

class Mage : public Unit
{
 public:
  using Unit::Unit;
  ~Mage() override = default;

  // but if we make it virtual then that's redundant because its already overridden...
  void start() override;
};
