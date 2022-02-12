// Created by Ben on 09/02/2022

#pragma once

#include "ASGEGameLib/Driver.hpp"

#include "ASGEGameLib/Components/Animator/Animator.hpp"

class Projectile : public bcw::Scene
{
 public:
  enum ProjectileColour
  {
    RED,
    GREEN
  };

  Projectile(
    std::shared_ptr<bcw::Scene> _target, const bcw::Vector2& start_position,
    const bcw::Vector2& _target_position, int _damage, ProjectileColour projectile_colour,
    float _projectile_speed, float projectile_scale);
  ~Projectile() override = default;

  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

 private:
  std::shared_ptr<bcw::Scene> target;
  int damage;

  std::unique_ptr<ASGE::Sprite> projectile_sprite;
  std::unique_ptr<bcw::Animator> animator;

  bcw::Vector2 target_position;
  float projectile_speed;
};
