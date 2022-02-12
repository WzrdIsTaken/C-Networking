// Created by Ben on 28/01/2022

#include <Engine/Logger.hpp>

#include "../../../../include/ASGEGameLib/Components/Networking/NetServerDebugger.hpp"

void bcw::networking::NetServerDebugger::onServerStartDebug()
{
  Logging::INFO("The server has started! At least we got this far...");
}

void bcw::networking::NetServerDebugger::onServerEndDebug()
{
  Logging::INFO("The server has closed");
}

void bcw::networking::NetServerDebugger::onSocketOpen(const kissnet::tcp_socket& /*client*/)
{
  Logging::INFO("A socket has opened");
}

void bcw::networking::NetServerDebugger::onSocketClose(const kissnet::tcp_socket& /*client*/)
{
  Logging::INFO("A socket has closed");
}

void bcw::networking::NetServerDebugger::onClientConnectDebug(
  const kissnet::tcp_socket& /*client*/, const int client_count)
{
  Logging::INFO(
    "A client has connected. " + std::to_string(client_count) + " clients are now connected");
}

void bcw::networking::NetServerDebugger::onClientDisconnectDebug(
  const kissnet::tcp_socket& /*client*/, const int client_count)
{
  Logging::INFO(
    "A client has disconnected" + std::to_string(client_count) + " clients are now connected");
}
