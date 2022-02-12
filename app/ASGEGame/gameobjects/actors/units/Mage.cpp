// Created by Ben on 04/02/2022

#include "Mage.hpp"

void Mage::start()
{
  // Stats
  unit_stats                 = UnitStats();
  unit_stats.start_hp        = 50;
  unit_stats.damage          = 50;
  unit_stats.move_distance   = 5;
  unit_stats.attack_range    = 7;
  unit_stats.projectile_size = 1.25F;

  // Animations
  std::vector<std::string> idle_animation;
  std::vector<std::string> move_animation;

  switch (team)
  {
    case Unit::DEMON:
      idle_animation = { "/data/actors/mage/team_one/necromancer_idle_anim_f0.png",
                         "/data/actors/mage/team_one/necromancer_idle_anim_f1.png",
                         "/data/actors/mage/team_one/necromancer_idle_anim_f2.png",
                         "/data/actors/mage/team_one/necromancer_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/mage/team_one/necromancer_run_anim_f0.png",
        "/data/actors/mage/team_one/necromancer_run_anim_f1.png",
        "/data/actors/mage/team_one/necromancer_run_anim_f2.png",
        "/data/actors/mage/team_one/necromancer_run_anim_f3.png",
      };
      break;
    case Unit::ORC:
      idle_animation = { "/data/actors/mage/team_two/orc_shaman_idle_anim_f0.png",
                         "/data/actors/mage/team_two/orc_shaman_idle_anim_f1.png",
                         "/data/actors/mage/team_two/orc_shaman_idle_anim_f2.png",
                         "/data/actors/mage/team_two/orc_shaman_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/mage/team_two/orc_shaman_run_anim_f0.png",
        "/data/actors/mage/team_two/orc_shaman_run_anim_f1.png",
        "/data/actors/mage/team_two/orc_shaman_run_anim_f2.png",
        "/data/actors/mage/team_two/orc_shaman_run_anim_f3.png",
      };
      break;
  }

  unit_animations = { { iDLE, idle_animation }, { MOVE, move_animation } };

  // Sets up the unit
  Unit::start();
  type = MAGE;

  // Sprite
  unit_sprite->loadTexture(unit_animations[iDLE].front());
  unit_sprite->setGlobalZOrder(1);
  unit_sprite->scale(2);
}
