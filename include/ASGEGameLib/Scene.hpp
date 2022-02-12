// Created by Ben on 20/01/2022

#pragma once

#include <Engine/GameTime.hpp>
#include <Engine/Gamepad.hpp>
#include <Engine/InputEvents.hpp>

// A scene can be like a scene in a tradition sense (think Unity), or a gameobject.
// It just makes things a little easier because in bcw scenes and gameobjects share so much
// functionality.

namespace bcw
{
  // --- Scene ---

  class Scene : public std::enable_shared_from_this<Scene> // https://bit.ly/3czkJq7
  {
   public:
    Scene()          = default;
    virtual ~Scene() = default;

    // Called when the scene is added or changed to
    virtual void start();
    // Called when a scene is removed
    virtual void end();

    // Called every frame if the scene is active and the game isn't paused
    virtual void update(const ASGE::GameTime& game_time);
    // Called every frame if the scene is active and the game isn't paused
    virtual void fixedUpdate(const ASGE::GameTime& game_time);
    // Called every frame if the scene is active
    virtual void render(const ASGE::GameTime& game_time);

    // Returns dont_destroy_on_load
    [[nodiscard]] bool getDontDestroyOnLoad() const;
    // Sets dont_destroy_on_load
    [[maybe_unused]] void setDontDestroyOnLoad(bool _dontDestroyOnLoad);

    // Returns active
    [[nodiscard]] bool getActiveState() const;
    // Sets active
    [[maybe_unused]] void setActiveState(bool _active);

    // Returns tag
    [[nodiscard]] std::string getTag() const;
    // Sets tag
    [[maybe_unused]] void setTag(const std::string& _tag);

    // Returns marked_for_deletion
    [[nodiscard]] bool getMarkedForDeletion() const;
    // This is kinda bot exposing this... so just please don't call it :) It's done for you!!
    void setMarkedForDeletion(bool _marked_for_deletion);

   private:
    // Determines if the object will be destroyed when the scene changes
    bool dont_destroy_on_load = false;
    // Determines if the scene's update and render functions will be called
    bool active = true;
    // Can be used to identify the scene for operations like collision or searching
    std::string tag = "None";

    // Determines if the scene will be deleted at the end of the frame
    bool marked_for_deletion = false;
  };

  // --- Interfaces ---

  class IKeyHandler
  {
   public:
    // Called when the user presses a key
    [[maybe_unused]] virtual void onKey(const ASGE::KeyEvent* key_event) = 0;
    virtual ~IKeyHandler()                                               = default;
  };

  class IClickHandler
  {
   public:
    // Called when the user clicks
    [[maybe_unused]] virtual void onMouse(const ASGE::ClickEvent* mouse_event) = 0;
    virtual ~IClickHandler()                                                   = default;
  };
} // namespace bcw
