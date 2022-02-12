// Created by Ben on 28/01/2022

#pragma once

#include "../include/ASGEGameLib/Driver.hpp"

#include <ASGEGameLib/Components/Networking/NetServerDebugger.hpp>

// A custom server debugger that (hopefully) makes life easier

class ServerDebugger
  : public bcw::Scene
  , public bcw::networking::NetServerDebugger
{
 public:
  ServerDebugger()           = default;
  ~ServerDebugger() override = default;

  void start() override;
  void render(const ASGE::GameTime& game_time) override;

 private:
  void onServerStartDebug() override;
  void onServerEndDebug() override;
  void onClientConnectDebug(const kissnet::tcp_socket& client, int client_count) override;
  void onClientDisconnectDebug(const kissnet::tcp_socket& client, int client_count) override;

  ASGE::Text server_online_text;
  ASGE::Text client_count_text;
};
