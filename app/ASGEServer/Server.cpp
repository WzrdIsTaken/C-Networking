// Created by James on 26/01/2022

#include "ServerDebugger.hpp"

#include "Server.hpp"

void Server::start()
{
  // why doesn't the template method work ):
  std::shared_ptr<bcw::networking::NetServerDebugger> server_debugger =
    dynamic_pointer_cast<bcw::networking::NetServerDebugger>(
      bcw::Driver::getSceneHandler()->addScene(std::make_shared<ServerDebugger>()));

  setServerDebugger(server_debugger);
  serverStart();

  game_state = LOBBY;
}

void Server::onClientConnect(kissnet::tcp_socket& player)
{
  if (client_count == 1)
  {
    bcw::networking::Packet packet;
    packet << PacketID::ONCONNECTIONESTABLISHED;
    packet << PlayerID::PLAYERONE;
    player.send(&packet.data()[0], packet.length());
  }
  if (client_count == 2)
  {
    bcw::networking::Packet packet;
    packet << PacketID::ONCONNECTIONESTABLISHED;
    packet << PlayerID::PLAYERTWO;
    player.send(&packet.data()[0], packet.length());
  }
}
/*
void Server::onClientDisconnect(const int client_count)
{
  if (client_count != 2 && !getAcceptConnections())
  {
    setAcceptConnections(true);
  }
}*/
