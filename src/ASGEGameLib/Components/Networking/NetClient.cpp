// Created by huxy on 23/02/2020
// Updated for use in bcw by James on 04/02/2022

#include <iostream>

#include "../../../../include/ASGEGameLib/Components/Networking/NetClient.hpp"

void bcw::networking::NetClient::update(const ASGE::GameTime& /*game_time*/) {}

void bcw::networking::NetClient::connect(const std::string& server_ip, unsigned short server_port)
{
  try
  {
    socket = (kissnet::endpoint{ server_ip, server_port });
    if (socket.connect())
    {
      workers.emplace_back(
        [&]
        {
          listen(socket);
          std::cout << "listening";
        });
    }
  }
  catch (std::runtime_error connection_to_server_failed)
  {
    std::cout << "Connection to server failed! Is the server online?";
  }
}

bcw::networking::NetClient::~NetClient()
{
  for (auto& thread : workers)
  {
    thread.detach();
  }

  socket.close();
}

void bcw::networking::NetClient::listen(kissnet::tcp_socket& csocket)
{
  bool continue_receiving = true;
  kissnet::buffer<4096> static_buffer;
  while (continue_receiving)
  {
    if (auto [size, valid] = csocket.recv(static_buffer); valid)
    {
      if (valid.value == kissnet::socket_status::cleanly_disconnected)
      {
        continue_receiving = false;
      }
      if (size < static_buffer.size())
        static_buffer[size] = std::byte{ 0 };

      auto bytes_received = static_buffer.size();

      packets.emplace(std::make_unique<Packet>());
      packets.back()->data().resize(bytes_received);

      std::copy(
        static_buffer.begin(),
        static_buffer.begin() + static_cast<ptrdiff_t>(bytes_received),
        &packets.back()->data()[0]);

      while (!packets.empty())
      {
        read(*packets.front());
        packets.pop();
      }
    }
    else
    {
      continue_receiving = false;
      csocket.close();
    }
  }
}

void bcw::networking::NetClient::read(bcw::networking::Packet& data)
{
  PacketID id;
  auto& packet = data;
  packet >> id;

  switch (id)
  {
    case PacketID::STRING:
      break;
    case PacketID::INT:
      break;
    case PacketID::FLOAT:
      break;
    case PacketID::ONCONNECTIONESTABLISHED:
    {
      packet >> my_id;
      if (my_id == PlayerID::PLAYERONE)
      {
        std::cout << "Player one"
                  << "\n";
      }
      else if (my_id == PlayerID::PLAYERTWO)
      {
        std::cout << "Player two"
                  << "\n";
      }
      break;
    }
    case PacketID::ONPLAYERCONNECT:
      // number of players
      int playercount;

      packet >> playercount;
      onPlayerConnect(playercount);
      std::cout << "connected";
      break;
    case PacketID::ONPLAYERDISCONNCT:
      onPlayerDisconnect(my_id, 1);
      break;
    case PacketID::ONPLAYERREADY:
      PlayerID new_id;
      int readycount;
      packet >> new_id;
      packet >> readycount;
      onPlayerReady(new_id, readycount);

      if (readycount == 2)
      {
        onGameStart();
      }
      break;
    case PacketID::ONPLAYERUNREADY:
      onPlayerUnready(1);
      break;
    case PacketID::ONPLAYERMOVE:
      PlayerID player_id;
      int team;
      int unit_id;
      float xpos;
      float ypos;
      packet >> player_id;
      packet >> team;
      packet >> unit_id;
      packet >> xpos;
      packet >> ypos;

      onUnitMove(player_id, team, unit_id, xpos, ypos);
      break;
    case PacketID::ONPLAYERATTACK:
      PlayerID pid;
      int attackerteam;
      int attackertype;
      int targetteam;
      int targettype;
      packet >> pid;
      packet >> attackerteam;
      packet >> attackertype;
      packet >> targetteam;
      packet >> targettype;
      onUnitAttack(pid, attackerteam, attackertype, targetteam, targettype);
      break;
    case PacketID::ONGAMEOVER:
      break;
    case PacketID::ONPLAYERWIN:
      PlayerID winningplayer;
      packet >> winningplayer;
      onPlayerWin(winningplayer);
      readycount = 0;
      break;
  }
}

void bcw::networking::NetClient::onPlayerConnect(int /*player_count*/) {}

void bcw::networking::NetClient::onPlayerDisconnect(PlayerID /*player_id*/, int /*player_count*/) {}

void bcw::networking::NetClient::onPlayerReady(PlayerID /*player_id*/, int /*ready_count*/) {}

void bcw::networking::NetClient::onPlayerUnready(int /*ready_count*/) {}

void bcw::networking::NetClient::onGameStart() {}
void bcw::networking::NetClient::onUnitMove(
  PlayerID /*id*/, int /*team*/, int /*type*/, float /*xpos*/, float /*ypos*/)
{
}
void bcw::networking::NetClient::onUnitAttack(
  PlayerID /*id*/, int /*attackerteam*/, int /*attackertype*/, int /*target_team*/,
  int /*target_type*/)
{
}

void bcw::networking::NetClient::onPlayerWin(PlayerID /*winning_player_id*/) {}
