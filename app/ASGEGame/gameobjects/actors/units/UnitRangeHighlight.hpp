// Created by Ben on 09/02/2022

#pragma once

#include "ASGEGameLib/Driver.hpp"

class UnitRangeHighlight : public bcw::Scene
{
 public:
  UnitRangeHighlight(ASGE::Sprite* _target, float range, const ASGE::Colour& colour);
  ~UnitRangeHighlight() override = default;

  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

  // Toggles the visibility of the highlight
  void toggleHighlight(bool toggle);

 private:
  // The sprites making up the box;
  std::unique_ptr<ASGE::Sprite> top;
  std::unique_ptr<ASGE::Sprite> bot;
  std::unique_ptr<ASGE::Sprite> left;
  std::unique_ptr<ASGE::Sprite> right;

  // The unit the highlight will be drawn around
  ASGE::Sprite* target;

  // How far away from the unit the sprites will be
  const float RANGE;

  // Is the box visible?
  bool show_highlight = false;
};

// Could use a vector in this script to reduce code repetition but I think having them
// as individuals makes things clearer

// This script also has some bot magic numbers, but we're getting close to the deadline
// and I'm well tired... The solution round this would basically be passing the tilemap
// in so we could use the world to tile / tile to world functions, but then I'd have to
// change like 2 functions and that's well hard
