// Created by Ben on 20/01/2022

#include "../../../include/ASGEGameLib/Handlers/SceneHandler.hpp"
#include "../../../include/ASGEGameLib/Driver.hpp"

void bcw::SceneHandler::update(const ASGE::GameTime& game_time)
{
  if (paused)
    return; // NOLINT one line

  should_delete_scenes = false;

  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->update(game_time);
    }
  }

  handleGarbageCollection();
}

void bcw::SceneHandler::fixedUpdate(const ASGE::GameTime& game_time)
{
  if (paused)
    return; // NOLINT one line

  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->fixedUpdate(game_time);
    }
  }
}

void bcw::SceneHandler::render(const ASGE::GameTime& game_time)
{
  for (const auto& scene : scenes)
  {
    if (scene->getActiveState())
    {
      scene->render(game_time);
    }
  }
}

void bcw::SceneHandler::changeScene(const std::shared_ptr<bcw::Scene>& new_scene)
{
  for (const auto& scene : scenes)
  {
    if (!scene->getDontDestroyOnLoad())
    {
      // scenes_to_delete.push(scene);
      removeScene(scene);
    }
  }

  addScene(new_scene);
}

std::shared_ptr<bcw::Scene> bcw::SceneHandler::addScene(const std::shared_ptr<bcw::Scene>& scene)
{
  Driver::getInputHandler()->addObservers(scene);
  scene->start();

  scenes.emplace_back(scene);
  return scene;
}

bool bcw::SceneHandler::removeScene(const std::shared_ptr<bcw::Scene>& scene)
{
  auto it = std::find(scenes.begin(), scenes.end(), scene);
  if (it != scenes.end())
  {
    scene->setActiveState(false);
    // Driver::getInputHandler()->removeObservers(scene);
    // todo fix this :p but doesn't seem to cause any problem
    scene->end();

    should_delete_scenes = true;
    scene->setMarkedForDeletion(true);
    return true;
  }

  return false;
}

void bcw::SceneHandler::handleGarbageCollection()
{
  if (should_delete_scenes)
  {
    auto it = scenes.begin();

    while (it != scenes.end())
    {
      if ((*it)->getMarkedForDeletion())
      {
        it = scenes.erase(it);
        // need to remove?
      }
      else
      {
        ++it;
      }
    }
  }
}

bool bcw::SceneHandler::getPausedState() const
{
  return paused;
}

void bcw::SceneHandler::setPausedState(const bool _paused) // NOLINT _paused
{
  paused = _paused;
}

std::shared_ptr<bcw::Scene> bcw::SceneHandler::findScene(const std::string& tag)
{
  for (const auto& scene : scenes)
  {
    if (scene->getTag() == tag)
    {
      return scene;
    }
  }

  return nullptr;
}

std::vector<std::shared_ptr<bcw::Scene>> bcw::SceneHandler::findScenes(const std::string& tag)
{
  std::vector<std::shared_ptr<bcw::Scene>> found_scenes;

  for (const auto& scene : scenes)
  {
    if (scene->getTag() == tag)
    {
      found_scenes.push_back(scene);
    }
  }

  return found_scenes;
}
