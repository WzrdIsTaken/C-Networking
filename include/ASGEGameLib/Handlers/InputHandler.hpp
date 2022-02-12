// Created by Ben on 20/01/2022

#pragma once

#include <Engine/InputEvents.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>

#include <map>

#include "../Scene.hpp"

#include "../../../include/ASGEGameLib/Containers/Float4.hpp"

// Handles mouse and keyboard input

namespace bcw
{
  class InputHandler
  {
   public:
    explicit InputHandler(const std::unique_ptr<ASGE::Input>& _asge_input, bool use_threads = false);
    ~InputHandler();

    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;

    // Checks if a click was on a sprite
    [[maybe_unused]] [[nodiscard]] bool
    checkClick(const ASGE::ClickEvent* mouse_event, const ASGE::Sprite& sprite);
    // Checks if a click was on text
    [[maybe_unused]] [[nodiscard]] bool
    checkClick(const ASGE::ClickEvent* mouse_event, const ASGE::Text& text);

    // Adds the scenes observers to the relevant map
    // NOTE: Called automatically when a scene is added
    void addObservers(const std::shared_ptr<bcw::Scene>& scene);
    // Removes the scenes observers from the relevant map
    // NOTE: Called automatically when a scene is removed
    void removeObservers(const std::shared_ptr<bcw::Scene>& scene);

   private:
    // Because I can't get std::variant to work ):
    // I didn't go for a template here because I don't want runtime error checking for the user
    [[nodiscard]] bool checkClick(
      const ASGE::ClickEvent* mouse_event, const bcw::Float4& bounds, int mouse_button = 0,
      int mouse_action = 1); // Left mouse button, press

    // Calls onKey on every IKeyHandler in the key_observers map
    void keyHandler(ASGE::SharedEventData data);
    // Calls onClick on every IClickHandler in the click_observers map
    void clickHandler(ASGE::SharedEventData data);

    // Calls [function] passing in [data] on every active observer of type [T]
    // Resources: Why mem_fn is best: https://bit.ly/3oHLrCD | Code examples: https://bit.ly/3kQMzTd
    template<typename T, typename U, typename V>
    void callObserverFunctionHelper(
      const std::map<std::shared_ptr<bcw::Scene>, std::shared_ptr<T>>& observer_map,
      const U& function, const V& data)
    {
      for (const auto& observer_pair : observer_map)
      {
        if (observer_pair.first->getActiveState())
        {
          // Create a magic variable that holds the function reference
          auto call_event = std::mem_fn(function);

          // Call the referenced function on the interface object passing in data
          call_event(observer_pair.second, data);
        }
      }
    }

    // Adds the passed in scene to the passed in observer map
    template<typename T>
    void addObserverHelper(
      std::map<std::shared_ptr<bcw::Scene>, std::shared_ptr<T>>& observer_map,
      const std::shared_ptr<bcw::Scene>& scene)
    {
      auto handler = std::dynamic_pointer_cast<T>(scene);
      {
        if (handler != nullptr)
        {
          observer_map.emplace(scene, handler);
        }
      }
    }

    // Removes the passed in scene from the passed in observer maps
    template<typename T>
    void removeObserversHelper(
      std::map<std::shared_ptr<bcw::Scene>, std::shared_ptr<T>>& observer_map,
      const std::shared_ptr<bcw::Scene>& scene)
    {
      if (observer_map.find(scene) != observer_map.end())
      {
        observer_map.erase(scene);
      }
    }

    // Game reference
    ASGE::Input* asge_input;

    // Callbacks
    std::string key_callback_id   = "yo dude~";
    std::string mouse_callback_id = "i'm a callback B)";

    // Event observers
    std::map<std::shared_ptr<bcw::Scene>, std::shared_ptr<bcw::IKeyHandler>> key_observers;
    std::map<std::shared_ptr<bcw::Scene>, std::shared_ptr<bcw::IClickHandler>> click_observers;
  };
} // namespace bcw
