// Created by James on 04/02/2022

#pragma once
#include "ASGEGameLib/Driver.hpp"
#include "gameobjects/network/Client.hpp"

class NetworkingTest : public bcw::Scene
{
 public:
  NetworkingTest()           = default;
  ~NetworkingTest() override = default;
  void update(const ASGE::GameTime& game_time) override;
  std::shared_ptr<Client> client;
  // std::shared_ptr<Client> client;

  void start() override;
};
