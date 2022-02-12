#include "ASGEGameLib/Driver.hpp"

#include "MainMenuScene.hpp"
// #include "NetworkingTest.hpp" set the initial scene as NetworkTest if your James doing magic

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_title  = "Client";
  game_settings.window_width  = 1280;
  game_settings.window_height = 720;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 30;
  game_settings.fps_limit     = 120;
  game_settings.msaa_level    = 16;

  bcw::Driver game(game_settings, std::make_shared<MainMenuScene>());
  return game.run();
}

// How to log:
// #include <Engine/Logger.hpp>
// Logging::INFO("Hello!");
