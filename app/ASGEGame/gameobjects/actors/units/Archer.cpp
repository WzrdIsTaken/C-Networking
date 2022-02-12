// Created by Ben on 04/02/2022

#include "Archer.hpp"

void Archer::start()
{
  // Stats
  unit_stats                 = UnitStats();
  unit_stats.start_hp        = 100;
  unit_stats.damage          = 30;
  unit_stats.move_distance   = 10;
  unit_stats.attack_range    = 8;
  unit_stats.projectile_size = 0.75F;

  // Animations
  std::vector<std::string> idle_animation;
  std::vector<std::string> move_animation;

  switch (team)
  {
    case Unit::DEMON:
      idle_animation = { "/data/actors/archer/team_one/wogol_idle_anim_f0.png",
                         "/data/actors/archer/team_one/wogol_idle_anim_f1.png",
                         "/data/actors/archer/team_one/wogol_idle_anim_f2.png",
                         "/data/actors/archer/team_one/wogol_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/archer/team_one/wogol_run_anim_f0.png",
        "/data/actors/archer/team_one/wogol_run_anim_f1.png",
        "/data/actors/archer/team_one/wogol_run_anim_f2.png",
        "/data/actors/archer/team_one/wogol_run_anim_f3.png",
      };
      break;
    case Unit::ORC:
      idle_animation = { "/data/actors/archer/team_two/orc_warrior_idle_anim_f0.png",
                         "/data/actors/archer/team_two/orc_warrior_idle_anim_f1.png",
                         "/data/actors/archer/team_two/orc_warrior_idle_anim_f2.png",
                         "/data/actors/archer/team_two/orc_warrior_idle_anim_f3.png" };
      move_animation = {
        "/data/actors/archer/team_two/orc_warrior_run_anim_f0.png",
        "/data/actors/archer/team_two/orc_warrior_run_anim_f1.png",
        "/data/actors/archer/team_two/orc_warrior_run_anim_f2.png",
        "/data/actors/archer/team_two/orc_warrior_run_anim_f3.png",
      };
      break;
  }

  unit_animations = { { iDLE, idle_animation }, { MOVE, move_animation } };

  // Setup unit
  Unit::start();
  type = ARCHER;

  // Sprite
  unit_sprite->loadTexture(unit_animations[iDLE].front());
  unit_sprite->setGlobalZOrder(1);
  unit_sprite->scale(2);
}
