// Created by Ben on 04/02/2022

#pragma once

#include <Engine/Sprite.hpp>

#include <map>
#include <vector>

#include "../../Component.hpp"

namespace bcw
{
  class Animator : public bcw::Component
  {
   public:
    [[maybe_unused]] Animator(
      ASGE::Sprite* _sprite, const std::map<std::string, std::vector<std::string>>& _animations,
      float _tick_speed, bool _loadingResourceFromDifferentThreadDumbFix = false);
    [[maybe_unused]] Animator(
      ASGE::Sprite* _sprite,
      const std::map<std::string, std::vector<std::array<float, 4>>>& _animations,
      float _tick_speed, bool _loadingResourceFromDifferentThreadDumbFix = false);
    ~Animator() override = default;

    // a n i m a t e
    void update(const ASGE::GameTime& game_time) override;

    // Returns the tick speed of the animation
    [[maybe_unused]] [[nodiscard]] float getTickSpeed() const;
    // Sets the tick speed of the animation
    [[maybe_unused]] void setTickSpeed(float new_speed);

    // Returns the current animation's name
    [[maybe_unused]] [[nodiscard]] std::string getAnimation() const;
    // Sets the animation via its string name.
    // Returns true if the current animation is already new_animation or if the switch was
    // successful. Returns false if new_animation doesn't exist
    [[maybe_unused]] bool setAnimation(const std::string& new_animation);

   private:
    enum AnimationMode
    {
      INDIVIDUAL,
      SPRITE_SHEET
    };
    const AnimationMode ANIMATION_MODE;

    ASGE::Sprite* sprite;
    std::map<std::string, std::vector<std::string>> individual_rendering_animations;
    std::map<std::string, std::vector<std::array<float, 4>>> sprite_sheet_rendering_animations;

    float tick_speed;
    float clock;

    unsigned long current_frame;
    std::string current_animation;

    bool loadingResourceFromDifferentThreadDumbFix = false;
  };
} // namespace bcw