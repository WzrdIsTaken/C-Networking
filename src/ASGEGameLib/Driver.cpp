// Created by Ben on 20/01/2022

#include "../../include/ASGEGameLib/Driver.hpp"

bcw::Driver* bcw::Driver::driver_ref; // NOLINT can't be const (i don't think...)

bcw::Driver::Driver(
  const ASGE::GameSettings& settings, const std::shared_ptr<bcw::Scene>& initial_scene) :
  OGLGame(settings)
{
  // A little bit of a round about way to get things going, but at least it works now...
  // So the static reference is needed to access the driver/handlers with just an #include
  // The handlers have to be created first, but any initialisations which will happen in
  // startServer() have to be done after the reference is assigned because otherwise we are trying
  // to access something that doesn't exist yet

  scene_handler  = std::make_unique<bcw::SceneHandler>();
  input_handler  = std::make_unique<bcw::InputHandler>(this->inputs);
  thread_handler = std::make_unique<bcw::ThreadHandler>();
  render_handler = std::make_unique<bcw::RenderHandler>(this->renderer);

  driver_ref = this;
  scene_handler->addScene(initial_scene);
}

bcw::Driver* bcw::Driver::getDriver()
{
  return driver_ref;
}

bcw::SceneHandler* bcw::Driver::getSceneHandler()
{
  return driver_ref->scene_handler.get();
}

bcw::InputHandler* bcw::Driver::getInputHandler()
{
  return driver_ref->input_handler.get();
}

bcw::ThreadHandler* bcw::Driver::getThreadHandler()
{
  return driver_ref->thread_handler.get();
}

bcw::RenderHandler* bcw::Driver::getRenderHandler()
{
  return driver_ref->render_handler.get();
}

ASGE::Renderer* bcw::Driver::getRenderer()
{
  return driver_ref->renderer.get();
}

void bcw::Driver::update(const ASGE::GameTime& game_time)
{
  scene_handler->update(game_time);
}

void bcw::Driver::fixedUpdate(const ASGE::GameTime& game_time)
{
  scene_handler->fixedUpdate(game_time);
}

void bcw::Driver::render(const ASGE::GameTime& game_time)
{
  scene_handler->render(game_time);
}