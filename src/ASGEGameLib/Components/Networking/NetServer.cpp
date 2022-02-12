// Created by huxy on 23/02/2020
// Updated for use in bcw by James on 04/02/2022

#include <iostream>

#include "../../../../include/ASGEGameLib/Components/Networking/NetServer.hpp"

// --- Magic ---

namespace
{
  kissnet::tcp_socket bind(const std::string& ip_address, kissnet::port_t port)
  {
    // create endpoint and assigning to a socket
    kissnet::tcp_socket server(kissnet::endpoint(ip_address, port));

    // bind and listen on
    server.bind();
    server.listen();

    // return it
    return server;
  }
} // namespace

bcw::networking::NetServer::~NetServer()
{
  accept_connections = false;
  bcw::Driver::getThreadHandler()->destroyThreads();
  server.close();
}

void bcw::networking::NetServer::setServerDebugger(
  const std::shared_ptr<NetServerDebugger>& debugger)
{
  net_server_debugger = debugger;
}

void bcw::networking::NetServer::serverStart()
{
  std::cout << "Server online!";

  server             = bind("0.0.0.0", 31276);
  accept_connections = true;

  if (net_server_debugger != nullptr)
  {
    net_server_debugger->onServerStartDebug();
  }

  // bcw::Driver::getThreadHandler()->scheduleJob();
  std::function<void()> func = [&]()
  {
    while (accept_connections)
    {
      auto& socket = connections.emplace_back(server.accept());

      std::cout << "connection received from: " << socket.get_recv_endpoint().address << ":"
                << socket.get_recv_endpoint().port << std::endl;
      // implement custom packets and pathfinding!!!!!!!!!!!!!!
      client_count++;
      onClientConnect(socket);
      Packet onconnect;
      onconnect << PacketID::ONPLAYERCONNECT;
      onconnect << client_count;
      send(onconnect);

      if (net_server_debugger != nullptr)
      {
        net_server_debugger->onClientConnectDebug(socket, client_count);
      }
      std::function<void()> receive = [&]()
      {
        listen(socket);

        client_count--;
        if (net_server_debugger != nullptr)
        {
          net_server_debugger->onClientDisconnectDebug(socket, client_count);
        }

        std::cout << "disconnect\n";
        if (const auto socket_iter =
              std::find(connections.begin(), connections.end(), std::ref(socket));
            socket_iter != connections.end())
        {
          std::cout << "closing socket...\n";
          connections.erase(socket_iter);
        }
      };
      workers.emplace_back(receive);
    }
  };

  workers.emplace_back(func);
  // workers.emplace_back(processdata);

  // bcw::Driver::getThreadHandler()->scheduleJob(func);

  // thread.detach();
}

void bcw::networking::NetServer::update(const ASGE::GameTime& /*game_time*/){

};

void bcw::networking::NetServer::listen(kissnet::tcp_socket& socket)
{
  bool continue_receiving = true;
  kissnet::buffer<4096> static_buffer;
  // std::queue<std::unique_ptr<Packet>> packets;
  while (continue_receiving)
  {
    if (auto [size, valid] = socket.recv(static_buffer); valid)
    {
      if (valid.value == kissnet::socket_status::cleanly_disconnected)
      {
        continue_receiving = false;
      }
      if (size < static_buffer.size())
      {
        static_buffer[size] = std::byte{ 0 };
      }

      auto bytes_received = static_buffer.size();

      packets.emplace(std::make_unique<Packet>());
      packets.back()->data().resize(bytes_received);
      // packet.data().capacity();

      std::copy(
        static_buffer.begin(),
        static_buffer.begin() + static_cast<ptrdiff_t>(bytes_received),
        &packets.back()->data()[0]);
      while (!packets.empty())
      {
        read(*packets.front());
        packets.pop();
      }

      // packet.data().resize(size);

      /*
      for (auto& client : connections)
      {
        client.send(static_buffer.data(), static_buffer.size());
      }*/
    }
    else
    {
      continue_receiving = false;
      // onClientDisconnect(client_count);
      socket.close();
    }
  }
}

void bcw::networking::NetServer::read(Packet& data)
{
  PacketID id;

  auto& packet = data;

  packet >> id;

  switch (id)
  {
    case PacketID::ONPLAYERUNREADY:
      break;
    case PacketID::ONPLAYERMOVE:
    {
      Packet newpacket;
      PlayerID pid;
      int unit;
      int team;
      float xpos;
      float ypos;
      packet >> pid;
      packet >> team;
      packet >> unit;
      packet >> xpos;
      packet >> ypos;
      newpacket << PacketID::ONPLAYERMOVE << pid << team << unit << xpos << ypos;
      send(newpacket);
      break;
    }
    case PacketID::ONPLAYERATTACK:
    {
      Packet newpacket;
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
      newpacket << PacketID::ONPLAYERATTACK << pid << attackerteam << attackertype << targetteam
                << targettype;
      send(newpacket);
    }
    break;
    case PacketID::ONGAMEOVER:
      break;
    case PacketID::ONPLAYERWIN:
    {
      // Let's go dude I actually wrote a packet this assignment - Ben 23:47 11/02/2022

      PlayerID winning_player_id;
      packet >> winning_player_id;

      Packet new_packet;
      new_packet << PacketID::ONPLAYERWIN;
      new_packet << winning_player_id;

      send(new_packet);
      break;
    }
    case PacketID::STRING:
    {
      std::string info;
      packet >> info;
      std::cout << info << "\n";
      break;
    }
    case PacketID::INT:
    {
      int info;
      packet >> info;
      std::cout << std::to_string(info) << "\n";
      break;
    }
    case PacketID::FLOAT:
    {
      break;
    }
    case PacketID::ONPLAYERCONNECT:
    {
      std::cout << "player connected";
      break;
    }
    case PacketID::ONCONNECTIONESTABLISHED:
    {
      std::cout << "connection established";
      break;
    }
    case PacketID::ONPLAYERDISCONNCT:
      break;
    case PacketID::ONPLAYERREADY:
      ready_count++;
      Packet newpacket;
      PlayerID player_id;
      packet >> player_id;
      newpacket << PacketID::ONPLAYERREADY;
      newpacket << player_id;
      newpacket << ready_count;
      send(newpacket);
      break;
  }
}

// void bcw::networking::NetServer::onClientDisconnect() {}

void bcw::networking::NetServer::setAcceptConnections(const bool _accept_connections)
{
  accept_connections = _accept_connections;
}

bool bcw::networking::NetServer::getAcceptConnections() const
{
  return accept_connections;
}
void bcw::networking::NetServer::onClientConnect(kissnet::tcp_socket& /*player*/) {}
