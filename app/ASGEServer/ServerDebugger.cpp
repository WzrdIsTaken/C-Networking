// Created by Ben on 28/01/2022

#include <ASGEGameLib/Containers/Vector2.hpp>

#include "ServerDebugger.hpp"

void ServerDebugger::start()
{
  const int FONT_INDEX = bcw::Driver::getRenderer()->loadFont("/data/text/KGHAPPY.ttf", 25);

  server_online_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_INDEX))
    .setString("Server Status: Offline")
    .setPosition(bcw::Vector2(10, 35));
  client_count_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_INDEX))
    .setString("Client Count: 0")
    .setPosition(bcw::Vector2(10, 70));
}

void ServerDebugger::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->renderText(server_online_text);
  bcw::Driver::getRenderer()->renderText(client_count_text);
}

void ServerDebugger::onServerStartDebug()
{
  bcw::networking::NetServerDebugger::onServerStartDebug();
  server_online_text.setString("Server Status: Online");
}

void ServerDebugger::onServerEndDebug()
{
  bcw::networking::NetServerDebugger::onServerEndDebug();
  server_online_text.setString("Server Status: Offline");
}

void ServerDebugger::onClientConnectDebug(
  const kissnet::tcp_socket& /*client*/, const int client_count)
{
  // bcw::networking::NetServerDebugger::onClientConnectDebug(client, client_count);
  client_count_text.setString("Client Count: " + std::to_string(client_count));
}

void ServerDebugger::onClientDisconnectDebug(
  const kissnet::tcp_socket& /*client*/, const int client_count)
{
  // bcw::networking::NetServerDebugger::onClientDisconnectDebug(client, client_count);
  client_count_text.setString("Client Count: " + std::to_string(client_count));
}
