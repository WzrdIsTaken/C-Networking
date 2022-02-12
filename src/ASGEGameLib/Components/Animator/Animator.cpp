// Created by Ben on 04/02/2022

#include <utility>

#include "../../../../include/ASGEGameLib/Components/Animator/Animator.hpp"

bcw::Animator::Animator(
  ASGE::Sprite* _sprite, const std::map<std::string, std::vector<std::string>>& _animations,
  float _tick_speed, bool _loadingResourceFromDifferentThreadDumbFix) :
  ANIMATION_MODE(INDIVIDUAL),
  sprite(_sprite), individual_rendering_animations(std::move(_animations)), // NOLINT ;_;
  tick_speed(_tick_speed), clock(tick_speed), current_frame(0),
  loadingResourceFromDifferentThreadDumbFix(_loadingResourceFromDifferentThreadDumbFix)
{
  current_animation = individual_rendering_animations.begin()->first; // Get the first item in
                                                                      // animations
}

bcw::Animator::Animator(
  ASGE::Sprite* _sprite,
  const std::map<std::string, std::vector<std::array<float, 4>>>& _animations, float _tick_speed,
  bool _loadingResourceFromDifferentThreadDumbFix) :
  ANIMATION_MODE(SPRITE_SHEET),
  sprite(_sprite), sprite_sheet_rendering_animations(std::move(_animations)), // NOLINT ;_;
  tick_speed(_tick_speed), clock(tick_speed), current_frame(0),
  loadingResourceFromDifferentThreadDumbFix(_loadingResourceFromDifferentThreadDumbFix)
{
  current_animation = sprite_sheet_rendering_animations.begin()->first;
}

void bcw::Animator::update(const ASGE::GameTime& game_time)
{
  clock += static_cast<float>(game_time.deltaInSecs());

  switch (ANIMATION_MODE)
  {
    case INDIVIDUAL:
      if (clock > tick_speed)
      {
        if (current_frame < individual_rendering_animations[current_animation].size() - 1)
        {
          current_frame++;
        }
        else
        {
          current_frame = loadingResourceFromDifferentThreadDumbFix ? 1 : 0;
        }

        std::string frame = individual_rendering_animations[current_animation][current_frame];

        // Old system was using src rects. Is this new way bad?
        bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(sprite, frame);

        clock = 0;
      }
      break;
    case SPRITE_SHEET:
      if (clock > tick_speed)
      {
        if (current_frame < sprite_sheet_rendering_animations[current_animation].size() - 1)
        {
          current_frame++;
        }
        else
        {
          current_frame = loadingResourceFromDifferentThreadDumbFix ? 1 : 0;
        }

        std::array<float, 4> frame =
          sprite_sheet_rendering_animations[current_animation][current_frame];
        sprite->srcRect()[0] = frame[0]; // Beginning x
        sprite->srcRect()[1] = frame[1]; // Beginning y
        sprite->srcRect()[2] = frame[2]; // Width
        sprite->srcRect()[3] = frame[3]; // Height

        clock = 0;
      }
      break;
  }
}

float bcw::Animator::getTickSpeed() const
{
  return tick_speed;
}

void bcw::Animator::setTickSpeed(const float _tick_speed) // NOLINT _tick_speed
{
  tick_speed = _tick_speed;
}

std::string bcw::Animator::getAnimation() const
{
  return current_animation;
}

bool bcw::Animator::setAnimation(const std::string& new_animation)
{
  if (current_animation == new_animation)
  {
    return true;
  }

  if (
    (ANIMATION_MODE == INDIVIDUAL && individual_rendering_animations.count(new_animation) > 0) ||
    (ANIMATION_MODE == SPRITE_SHEET && sprite_sheet_rendering_animations.count(new_animation) > 0))
  {
    current_animation = new_animation;
    clock             = tick_speed + 1;
    current_frame     = 0;

    return true;
  }

  return false;
}
