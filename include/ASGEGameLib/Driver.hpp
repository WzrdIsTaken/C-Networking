// Created by Ben on 20/01/2022

#pragma once

#include <Engine/OGLGame.hpp>

#include "Handlers/InputHandler.hpp"
#include "Handlers/RenderHandler.hpp"
#include "Handlers/SceneHandler.hpp"
#include "Handlers/ThreadHandler.hpp"

// The class carrying the rest of the project. Implements a slightly... interesting? design pattern

namespace bcw
{
  class Driver : public ASGE::OGLGame
  {
   public:
    explicit Driver(
      const ASGE::GameSettings& settings, const std::shared_ptr<bcw::Scene>& initial_scene);
    ~Driver() override = default;

    Driver(const Driver&) = delete;
    Driver& operator=(const Driver&) = delete;

    // In case you need to do anything 'ASGEy'. Much easier than writing a bunch of getters...
    [[maybe_unused]] static Driver* getDriver();
    // Returns the scene_handler
    [[maybe_unused]] static SceneHandler* getSceneHandler();
    // Returns the input_handler
    [[maybe_unused]] static InputHandler* getInputHandler();
    // Returns the thread_handler
    [[maybe_unused]] static ThreadHandler* getThreadHandler();
    // Returns the render_handler [not the same as getRenderer! - that's asge's thing!]
    [[maybe_unused]] static RenderHandler* getRenderHandler();

    // Returns ASGE's renderer
    [[maybe_unused]] static ASGE::Renderer* getRenderer();

   private:
    // Called every frame. Updates all handlers which require it
    void update(const ASGE::GameTime& game_time) override;
    // Called every physic timestep. FixedUpdates all handlers which require it
    void fixedUpdate(const ASGE::GameTime& game_time) override;
    // Called every frame. Renders all handlers which require it
    void render(const ASGE::GameTime& game_time) override;

    // Handlers
    std::unique_ptr<bcw::SceneHandler> scene_handler;
    std::unique_ptr<bcw::InputHandler> input_handler;
    std::unique_ptr<bcw::ThreadHandler> thread_handler;
    std::unique_ptr<bcw::RenderHandler> render_handler;

    // I think this is still a bot idea. Well, at least it will reduce code repetition...
    static Driver* driver_ref; // NOLINT can't be const (i don't think...)
  };
} // namespace bcw
