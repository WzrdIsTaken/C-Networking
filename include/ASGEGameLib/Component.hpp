// Created by huxy on 23/02/2020
// Updated for use in bcw by Ben on 25/01/2022

#pragma once

#include <Engine/GameTime.hpp>

#include "Driver.hpp"

// Base component class that all components inherit from

namespace bcw
{
  class Component
  {
   public:
    Component()          = default;
    virtual ~Component() = default;

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    virtual void update(const ASGE::GameTime& game_time);
  };
} // namespace bcw

// ngl some components in the component's folder aren't actually components but i don't have a
// better name for them rn
