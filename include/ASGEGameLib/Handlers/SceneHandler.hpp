// Created by Ben on 20/01/2022

#pragma once

#include <queue>
#include <vector>

#include "../Scene.hpp"

// Handles the creation and deletion of scenes (/gameobjects)

namespace bcw
{
  class SceneHandler
  {
   public:
    SceneHandler()  = default;
    ~SceneHandler() = default;

    SceneHandler(const SceneHandler&) = delete;
    SceneHandler& operator=(const SceneHandler&) = delete;

    // Calls update on all active scenes (if paused is false)
    void update(const ASGE::GameTime& game_time);
    // Calls fixedUpdates on all active scenes (if paused is false)
    void fixedUpdate(const ASGE::GameTime& game_time);
    // Calls render on all active scenes (regardless is paused is true or false)
    void render(const ASGE::GameTime& game_time);

    // Deletes all current scenes not marked as don't destroy on load and loads the new_scene
    void changeScene(const std::shared_ptr<bcw::Scene>& new_scene);

    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> changeScene()
    {
      std::shared_ptr<T> scene = std::make_shared<T>();
      return changeScene(scene);
    }

    // Adds a new scene to the scenes vector
    std::shared_ptr<bcw::Scene> addScene(const std::shared_ptr<bcw::Scene>& scene);

    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> addScene()
    {
      std::shared_ptr<T> scene = std::make_shared<T>();
      return addScene(scene);
    }

    // Tries to delete the scene from the scenes vector, returning true/false depending upon if it
    // was a success
    [[maybe_unused]] bool removeScene(const std::shared_ptr<bcw::Scene>& scene);

    // Finds a scene based off its tag
    [[maybe_unused]] std::shared_ptr<bcw::Scene> findScene(const std::string& tag);

    // Finds all the scenes with a tag
    [[maybe_unused]] std::vector<std::shared_ptr<bcw::Scene>> findScenes(const std::string& tag);

    // Finds a scene based off its type
    template<typename T>
    [[maybe_unused]] std::shared_ptr<T> findScene()
    {
      for (const auto& scene : scenes)
      {
        auto scene_of_type = std::dynamic_pointer_cast<T>(scene);
        if (scene_of_type != nullptr)
        {
          return scene_of_type;
        }
      }

      return nullptr;
    }

    // Finds all scenes of a type
    template<typename T>
    [[maybe_unused]] std::vector<std::shared_ptr<T>> findScenes()
    {
      std::vector<std::shared_ptr<T>> found_scenes;

      for (const auto& scene : scenes)
      {
        auto scene_of_type = std::dynamic_pointer_cast<T>(scene);
        if (scene_of_type != nullptr)
        {
          found_scenes.push_back(scene_of_type);
        }
      }

      return found_scenes;
    }

    // Returns paused
    [[maybe_unused]] [[nodiscard]] bool getPausedState() const;
    // Sets paused
    [[maybe_unused]] void setPausedState(bool _paused);

   private:
    // Deletes all marked scenes at the end of the frame
    void handleGarbageCollection();

    // Scenes
    std::vector<std::shared_ptr<bcw::Scene>> scenes;

    // Controls whether scenes are updated
    bool paused = false;

    // Are their scenes waiting to be deleted?
    bool should_delete_scenes = false;
  };
} // namespace bcw
