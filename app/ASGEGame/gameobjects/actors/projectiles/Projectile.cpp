// Created by Ben on 09/02/2022

#include "Projectile.hpp"

#include "../units/Unit.hpp"

#include <cmath>
#include <utility>

Projectile::Projectile(
  std::shared_ptr<bcw::Scene> _target, const bcw::Vector2& start_position,
  const bcw::Vector2& _target_position, const int _damage, // NOLINT clang making my comments not
                                                           // stay on the
  const ProjectileColour projectile_colour, const float _projectile_speed, // NOLINT same line...
  const float projectile_scale) :
  target(std::move(_target)),
  damage(_damage), target_position(_target_position), projectile_speed(_projectile_speed)
{
  std::string path = "/data/actors/";
  switch (projectile_colour)
  {
    case ProjectileColour::RED:
      path += "fireball_red/fireball_red";
      break;
    case ProjectileColour::GREEN:
      path += "fireball_green/fb_green";
      break;
  }

  std::vector<std::string> animation = {
    path + "_f0.png", path + "_f1.png", path + "_f2.png", path + "_f3.png", path + "_f4.png"
  };
  std::map<std::string, std::vector<std::string>> animations = { { "Default", animation } };

  projectile_sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    projectile_sprite.get(), animation.front(), start_position, projectile_scale, 2);

  float dx  = target_position.x - start_position.x;
  float dy  = target_position.y - start_position.y;
  float rad = atan2(dy, dx);
  projectile_sprite->rotationInRadians(rad);

  animator = std::make_unique<bcw::Animator>(projectile_sprite.get(), animations, 0.12F, true);
}

void Projectile::update(const ASGE::GameTime& game_time)
{
  animator->update(game_time);

  bcw::Vector2 current_position =
    bcw::Vector2(projectile_sprite->xPos(), projectile_sprite->yPos());

  if (current_position.distance(target_position) > 1)
  {
    bcw::Vector2 move_direction =
      bcw::Vector2(target_position.x - current_position.x, target_position.y - current_position.y)
        .normalise();

    bcw::Vector2 new_position = bcw::Vector2(
      (current_position.x + move_direction.x) * projectile_speed,
      (current_position.y + move_direction.y) * projectile_speed);
    // * by dt!

    projectile_sprite->xPos(new_position.x);
    projectile_sprite->yPos(new_position.y);
  }
  else
  {
    dynamic_pointer_cast<Unit>(target)->takeDamage(damage);
    bcw::Driver::getSceneHandler()->removeScene(this->shared_from_this());
  }
}

void Projectile::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->render(*projectile_sprite);
}
