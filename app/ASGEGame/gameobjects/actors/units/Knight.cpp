// Created by Ben on 04/02/2022

#include "Knight.hpp"

void Knight::start()
{
  // Stats
  unit_stats                  = UnitStats();
  unit_stats.start_hp         = 300;
  unit_stats.damage           = 10;
  unit_stats.move_distance    = 5;
  unit_stats.attack_range     = 3;
  unit_stats.projectile_size  = 0.5F;
  unit_stats.hp_text_x_offset = 25; // bot solution

  // Animations
  std::vector<std::string> idle_animation;
  std::vector<std::string> move_animation;

  switch (team)
  {
    case Unit::DEMON:
      idle_animation = { "/data/actors/knight/team_one/big_demon_idle_anim_f0.png",
                         "/data/actors/knight/team_one/big_demon_idle_anim_f1.png",
                         "/data/actors/knight/team_one/big_demon_idle_anim_f2.png",
                         "/data/actors/knight/team_one/big_demon_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/knight/team_one/big_demon_run_anim_f0.png",
        "/data/actors/knight/team_one/big_demon_run_anim_f1.png",
        "/data/actors/knight/team_one/big_demon_run_anim_f2.png",
        "/data/actors/knight/team_one/big_demon_run_anim_f3.png",
      };
      break;
    case Unit::ORC:
      idle_animation = { "/data/actors/knight/team_two/ogre_idle_anim_f0.png",
                         "/data/actors/knight/team_two/ogre_idle_anim_f1.png",
                         "/data/actors/knight/team_two/ogre_idle_anim_f2.png",
                         "/data/actors/knight/team_two/ogre_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/knight/team_two/ogre_run_anim_f0.png",
        "/data/actors/knight/team_two/ogre_run_anim_f1.png",
        "/data/actors/knight/team_two/ogre_run_anim_f2.png",
        "/data/actors/knight/team_two/ogre_run_anim_f3.png",
      };
      break;
  }

  unit_animations = { { iDLE, idle_animation }, { MOVE, move_animation } };

  // Set up the unit
  Unit::start();
  type = KNIGHT;

  // Sprite
  unit_sprite->loadTexture(unit_animations[iDLE].front());
  unit_sprite->setGlobalZOrder(1);
  unit_sprite->scale(2);
}
