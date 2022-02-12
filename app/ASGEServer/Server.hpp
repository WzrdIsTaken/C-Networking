// Created by James on 26/01/2022

#pragma once

#include "ServerDebugger.hpp"
#include <ASGEGameLib/Components/Networking/NetServer.hpp>

// The custom server class specific for this game

class Server
  : public bcw::Scene
  , public bcw::networking::NetServer
{
 public:
  Server()           = default;
  ~Server() override = default;

  // Starts the server
  void start() override;

 protected:
  void onClientConnect(kissnet::tcp_socket& socket) override;
  // void onClientDisconnect(int client_count) override;

 private:
  enum GameState : int
  {
    LOBBY,
    GAME
  };
  GameState game_state;
};
