// Created by Ben on 04/02/2022

#pragma once

#include "ASGEGameLib/Components/Animator/Animator.hpp"
#include "ASGEGameLib/Driver.hpp"

#include "UnitRangeHighlight.hpp"

class Unit : public bcw::Scene
{
 public:
  // The stats of the unit. Loading these from a file might be cool?
  struct UnitStats
  {
    int move_distance = 0;
    float move_speed  = 1;
    int attack_range  = 0;
    int damage        = 0;
    int start_hp      = 0;

    float projectile_speed = 1;
    float projectile_size  = 1;

    // dude i know this isn't a stat but my calc for centering text over the head of the sprite
    // isn't working and i have no idea why so will just use this bot solution for now ;_;
    float hp_text_x_offset = 10;
  };

  // What team the unit is on
  enum Team : int
  {
    DEMON,
    ORC
  };

  // What type the unit is
  enum Type : int
  {
    KNIGHT,
    ARCHER,
    MAGE
  };

  Unit(std::shared_ptr<bcw::Scene> _game, const bcw::Vector2& _start_position, Team _team);
  ~Unit() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;

  // Makes the unit attack another. Could be overridden in each unit for a different type of attack,
  // but in this simple game only fireballs are used
  void attack(const std::shared_ptr<Unit>& target);
  // Sends a path that the unit will follow move
  void sendPath(const std::vector<bcw::Vector2>& _path);
  // Reduces the units hp and does other health related shenanigans
  void takeDamage(int damage);

  // Called when the unit is selected
  void select(bool selected);

  // Gets the units sprite
  ASGE::Sprite* getSprite() const;
  // Gets the team the unit is on
  Unit::Team getTeam() const;
  // Gets the unit of the unit
  Unit::Type getType() const;
  // Gets unit_stats
  Unit::UnitStats getUnitStats() const;

 protected:
  UnitStats unit_stats;

  // The units sprite
  std::unique_ptr<ASGE::Sprite> unit_sprite;
  // The units animations
  std::map<std::string, std::vector<std::string>> unit_animations;

  // The unit's animation names
  const std::string iDLE = "idle"; // NOLINT so no shadow :p
  const std::string MOVE = "move";

  // The units team
  Team team;
  // The units type
  Type type;

 private:
  // Updates the position of hp_text
  void updateHealthTextPosition();

  // What the unit is currently doing
  enum UnitState
  {
    IDLE,
    MOVING_ALONG_PATH
  };
  UnitState unit_state = IDLE;

  // A bit of dependency injection because its easy, and we are running out of time...
  std::shared_ptr<bcw::Scene> game;

  // The path a unit will follow
  std::vector<bcw::Vector2> path;

  // The units current hp
  int hp = 0;

  // The units animator
  std::unique_ptr<bcw::Animator> animator;

  // Displays how much hp the unit has
  ASGE::Text hp_text;
  // The distance above the units head that the health text should go
  bcw::Vector2 hp_text_offset;

  // A highlight around the unit which displays how far they can move
  std::shared_ptr<UnitRangeHighlight> move_highlight;
  // A highlight around the unit which displays how far they can attack
  std::shared_ptr<UnitRangeHighlight> attack_highlight;

  // This is a little bot, but I can't think of a cleaner way rn due to the constructor / start
  // problem
  bcw::Vector2 start_position;
};
