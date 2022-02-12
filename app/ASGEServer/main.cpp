// Created by Ben on 28/01/2022

#include "ASGEGameLib/Driver.hpp"

#include "Server.hpp"

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_title  = "Server";
  game_settings.window_width  = 1280;
  game_settings.window_height = 720;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 30;
  game_settings.fps_limit     = 120;
  game_settings.msaa_level    = 16;

  bcw::Driver game(game_settings, std::make_shared<Server>());
  return game.run();
}
