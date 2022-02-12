// Created by James on 04/02/2022

#include "NetworkingTest.hpp"

void NetworkingTest::start()
{
  client = std::dynamic_pointer_cast<Client>(
    bcw::Driver::getSceneHandler()->addScene(std::make_shared<Client>()));
  // client->sendint(5);
}

void NetworkingTest::update(const ASGE::GameTime& /* game_time*/) {}
