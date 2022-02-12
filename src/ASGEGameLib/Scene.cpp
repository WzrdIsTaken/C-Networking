// Created by Ben on 20/01/2022

#include "../../include/ASGEGameLib/Scene.hpp"

void bcw::Scene::start() {}

void bcw::Scene::end() {}

void bcw::Scene::update(const ASGE::GameTime& /*game_time*/) {}

void bcw::Scene::fixedUpdate(const ASGE::GameTime& /*game_time*/) {}

void bcw::Scene::render(const ASGE::GameTime& /*game_time*/) {}

bool bcw::Scene::getDontDestroyOnLoad() const
{
  return dont_destroy_on_load;
}

void bcw::Scene::setDontDestroyOnLoad(const bool _dont_destroy_on_load) // NOLINT
                                                                        // _dont_destroy_on_load
{
  dont_destroy_on_load = _dont_destroy_on_load;
}

bool bcw::Scene::getActiveState() const
{
  return active;
}

void bcw::Scene::setActiveState(const bool _active) // NOLINT _active
{
  active = _active;
}

std::string bcw::Scene::getTag() const
{
  return tag;
}

void bcw::Scene::setTag(const std::string& _tag)
{
  tag = _tag;
}

bool bcw::Scene::getMarkedForDeletion() const
{
  return marked_for_deletion;
}

void bcw::Scene::setMarkedForDeletion(const bool _marked_for_deletion) // NOLINT
                                                                       // _marked_for_deletion
{
  marked_for_deletion = _marked_for_deletion;
}
