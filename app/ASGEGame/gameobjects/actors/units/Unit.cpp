// Created by Ben on 04/02/2022

#include "Unit.hpp"

#include "../../../GigaScene.hpp"
#include "../projectiles/Projectile.hpp"

#include <utility>

Unit::Unit( // NOLINTNEXTLINE 그냥
  std::shared_ptr<bcw::Scene> _game, const bcw::Vector2& _start_position, const Team _team) :
  team(_team),
  game(std::move(_game)), start_position(_start_position) // NOLINT
{
}

void Unit::start()
{
  // Sprite
  unit_sprite = bcw::Driver::getRenderer()->createUniqueSprite();
  unit_sprite->xPos(start_position.x);
  unit_sprite->yPos(start_position.y);

  if (start_position.x > bcw::Driver::getRenderer()->windowWidth() / 2)
  {
    unit_sprite->setFlipFlags(ASGE::Sprite::FLIP_X);
  }

  animator = std::make_unique<bcw::Animator>(unit_sprite.get(), unit_animations, 0.12F);

  // Stats
  hp = unit_stats.start_hp;

  // Text
  const int FONT = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 15);
  hp_text.setFont(bcw::Driver::getRenderer()->getFont(FONT))
    .setString(std::to_string(hp))
    .setZOrder(4)
    .setColour(ASGE::COLOURS::RED);

  hp_text_offset = bcw::Vector2(unit_stats.hp_text_x_offset, 10);
  updateHealthTextPosition();

  // Highlight
  move_highlight = std::dynamic_pointer_cast<UnitRangeHighlight>(
    bcw::Driver::getSceneHandler()->addScene(std::make_shared<UnitRangeHighlight>(
      unit_sprite.get(), unit_stats.move_distance, ASGE::Colour(0, 0, 255))));
  attack_highlight = std::dynamic_pointer_cast<UnitRangeHighlight>(
    bcw::Driver::getSceneHandler()->addScene(std::make_shared<UnitRangeHighlight>(
      unit_sprite.get(), unit_stats.attack_range, ASGE::Colour(0, 255, 0))));
}

void Unit::update(const ASGE::GameTime& game_time)
{
  animator->update(game_time);

  switch (unit_state)
  {
    case IDLE:
      break;
    case MOVING_ALONG_PATH:

      if (!path.empty())
      {
        bcw::Vector2 target_position  = path.back();
        bcw::Vector2 current_position = bcw::Vector2(unit_sprite->xPos(), unit_sprite->yPos());

        if (current_position.distance(target_position) > 1)
        {
          bcw::Vector2 move_direction =
            bcw::Vector2(
              target_position.x - current_position.x, target_position.y - current_position.y)
              .normalise();

          // auto speed = static_cast<float>(move_speed * game_time.deltaInSecs());
          float speed = unit_stats.move_speed; // todo: for some reason speed doesn't like being
                                               // multiplied??

          bcw::Vector2 new_position = bcw::Vector2(
            (current_position.x + move_direction.x) * speed,
            (current_position.y + move_direction.y) * speed);

          unit_sprite->xPos(new_position.x);
          unit_sprite->yPos(new_position.y);

          updateHealthTextPosition();
        }
        else
        {
          path.pop_back();
        }
      }
      else
      {
        unit_state = IDLE;
        animator->setAnimation(iDLE);

        dynamic_pointer_cast<GigaScene>(game)->gameSetGameState(GigaScene::GameGameState::DEFAULT);
      }
      break;
  }
}

void Unit::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->renderSprite(*unit_sprite);
  bcw::Driver::getRenderer()->renderText(hp_text);
}

void Unit::attack(const std::shared_ptr<Unit>& target)
{
  bcw::Vector2 current_position = bcw::Vector2(unit_sprite->xPos(), unit_sprite->yPos());
  bcw::Vector2 target_position =
    bcw::Vector2(target->getSprite()->xPos(), target->getSprite()->yPos());

  bcw::Driver::getSceneHandler()->addScene(std::make_shared<Projectile>(
    target,
    current_position,
    target_position,
    unit_stats.damage,
    team == DEMON ? Projectile::ProjectileColour::RED : Projectile::ProjectileColour::GREEN,
    unit_stats.projectile_speed,
    unit_stats.projectile_size));
}

void Unit::sendPath(const std::vector<bcw::Vector2>& _path)
{
  if (_path.empty())
  {
    return;
  }

  if (_path.begin()->x < unit_sprite->xPos() && !unit_sprite->isFlippedOnX())
  {
    unit_sprite->setFlipFlags(ASGE::Sprite::FLIP_X);
  }
  else if (_path.begin()->x > unit_sprite->xPos() && unit_sprite->isFlippedOnX())
  {
    unit_sprite->setFlipFlags(ASGE::Sprite::NORMAL);
  }

  path = _path;
  animator->setAnimation(MOVE);
  unit_state = MOVING_ALONG_PATH;
}

void Unit::takeDamage(int damage)
{
  hp -= damage;
  hp_text.setString(std::to_string(hp));

  if (hp < 1)
  {
    dynamic_pointer_cast<GigaScene>(game)->gameKillUnit(
      dynamic_pointer_cast<Unit>(this->shared_from_this())); // ah ?@???
    bcw::Driver::getSceneHandler()->removeScene(this->shared_from_this());
  }
}

void Unit::select(const bool selected)
{
  move_highlight->toggleHighlight(selected);
  attack_highlight->toggleHighlight(selected);
}

void Unit::updateHealthTextPosition()
{
  hp_text.setPosition(
    bcw::Vector2(unit_sprite->xPos() + hp_text_offset.x, unit_sprite->yPos() - hp_text_offset.y));
}

ASGE::Sprite* Unit::getSprite() const
{
  return unit_sprite.get();
}

Unit::Team Unit::getTeam() const
{
  return team;
}

Unit::Type Unit::getType() const
{
  return type;
}

Unit::UnitStats Unit::getUnitStats() const
{
  return unit_stats;
}
