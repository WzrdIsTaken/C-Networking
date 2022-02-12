// Created by Ben on 01/02/2022

#pragma once

#include <Engine/Renderer.hpp>

#include "../../../include/ASGEGameLib/Containers/Vector2.hpp"
#include <Engine/Sprite.hpp>
#include <variant>

// Handles everything to do with our :eyes:

namespace bcw
{
  class RenderHandler
  {
   public:
    explicit RenderHandler(const std::unique_ptr<ASGE::Renderer>& _asge_renderer);
    ~RenderHandler() = default;

    RenderHandler(const RenderHandler&) = delete;
    RenderHandler& operator=(const RenderHandler&) = delete;

    // Helps to reduce code repetition
    // ASGE::Text supports method chaining... ASGE::Sprite's do not ):
    [[maybe_unused]] ASGE::Sprite* setupSprite(
      ASGE::Sprite* sprite, const std::string& texture,
      const bcw::Vector2& position = bcw::Vector2(0, 0), float scale = 1, short z_order = 1,
      ASGE::GameSettings::MagFilter mag_filter = ASGE::GameSettings::MagFilter::NEAREST);

    // It seems that when a new sprite texture is loaded, the mag filter is reset.
    // This function makes life a little easier
    [[maybe_unused]] ASGE::Sprite* loadSpriteTextureAndMaintainMagFilter(
      ASGE::Sprite* sprite, const std::string& texture,
      ASGE::GameSettings::MagFilter mag_filter = ASGE::GameSettings::MagFilter::NEAREST);

    // Returns the centre of the screen for text
    // If ScreenCentre is X or Y, then other_axis_value will supply the other value.
    enum ScreenCentre
    {
      X,
      Y,
      BOTH
    };
    [[maybe_unused]] [[nodiscard]] bcw::Vector2 getScreenCentre(
      ScreenCentre screen_centre, const ASGE::Text& text, float other_axis_value = INFINITY);
    // Returns the centre of the screen for sprites
    [[maybe_unused]] [[nodiscard]] bcw::Vector2 getScreenCentre(
      ScreenCentre screen_centre, const ASGE::Sprite& sprite, float other_axis_value = INFINITY);

   private:
    // Because I can't get std::variant to work ):
    // I didn't go for a template here because I don't want runtime error checking for the user
    [[nodiscard]] bcw::Vector2 actuallyGetScreenCentre(
      ScreenCentre screen_centre, bcw::Vector2 bounds, float other_axis_value = INFINITY);

    // Game reference
    ASGE::Renderer* asge_renderer;
  };
} // namespace bcw

// In bcw asge v1, I had a lot of functions to handle text / sprite rendering and everything
// was handled behind the scenes. However, this got a little messy, so I am trying out a different
// approach this time. The actual rendering will be in done in the individual scenes, and this
// script will just contain some helper methods.
