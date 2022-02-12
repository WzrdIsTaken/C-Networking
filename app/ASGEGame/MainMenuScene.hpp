// Created by Ben on 20/01/2022

#pragma once

#include "../include/ASGEGameLib/Driver.hpp"

// Allows the user to navigate through the initial list of options

class MainMenuScene
  : public bcw::Scene
  , public bcw::IClickHandler
{
 public:
  void start() override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

 private:
  std::unique_ptr<ASGE::Sprite> background_image;

  ASGE::Text title_text;
  ASGE::Text play_text;
  ASGE::Text exit_text;
};
