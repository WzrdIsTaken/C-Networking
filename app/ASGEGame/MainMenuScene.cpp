// Created by Ben on 20/01/2022

#include <ASGEGameLib/Containers/Vector2.hpp>

#include "GigaScene.hpp"
#include "MainMenuScene.hpp"

void MainMenuScene::start()
{
  // Background
  background_image = bcw::Driver::getRenderer()->createUniqueSprite();
  background_image->loadTexture("/data/ui/MenuBackground.png");

  // Text
  const int FONT_BIG   = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 75);
  const int FONT_SMALL = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 50);

  title_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_BIG))
    .setString("Super Cool Network Game")
    .setPosition(
      bcw::Driver::getRenderHandler()->getScreenCentre(bcw::RenderHandler::X, title_text, 200));
  play_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Play")
    .setPosition(
      bcw::Driver::getRenderHandler()->getScreenCentre(bcw::RenderHandler::X, play_text, 400));
  exit_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Exit")
    .setPosition(
      bcw::Driver::getRenderHandler()->getScreenCentre(bcw::RenderHandler::X, play_text, 475));
}

void MainMenuScene::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->renderSprite(*background_image);

  bcw::Driver::getRenderer()->renderText(title_text);
  bcw::Driver::getRenderer()->renderText(play_text);
  bcw::Driver::getRenderer()->renderText(exit_text);
}

void MainMenuScene::onMouse(const ASGE::ClickEvent* mouse_event)
{
  if (bcw::Driver::getInputHandler()->checkClick(mouse_event, play_text))
  {
    bcw::Driver::getSceneHandler()->changeScene(std::make_shared<GigaScene>());
  }
  if (bcw::Driver::getInputHandler()->checkClick(mouse_event, exit_text))
  {
    bcw::Driver::getDriver()->signalExit();
  }
}
