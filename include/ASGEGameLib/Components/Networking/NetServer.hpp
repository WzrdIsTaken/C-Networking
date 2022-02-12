// Created by huxy on 23/02/2020
// Updated for use in bcw by James on 04/02/2022

#pragma once

#include <kissnet.hpp>

#include "../../../../include/ASGEGameLib/Components/Networking/Packet.hpp"
#include "../../Driver.hpp"
#include "NetServerDebugger.hpp"
#include <atomic>
#include <list>
#include <queue>
#include <thread>
#include <vector>
// The base class of a TCP server that can be connected to.
// Has a bunch of virtual callbacks that can be overridden to implement custom functionality.

namespace bcw::networking
{
  class NetServer
  {
   public:
    NetServer() = default;
    virtual ~NetServer();

    NetServer(const NetServer&) = delete;
    NetServer& operator=(const NetServer&) = delete;

    //
    void serverStart();
    //
    void update(const ASGE::GameTime& game_time);
    // Sets up the server debugger
    void setServerDebugger(const std::shared_ptr<NetServerDebugger>& debugger);

    void read(Packet& data);

    void send(Packet& packet)
    {
      for (auto& client : connections)
      {
        client.send(&packet.data()[0], packet.length());
      }
    }

    void setAcceptConnections(bool _accept_connections);
    bool getAcceptConnections() const;

   protected:
    virtual void onClientConnect(kissnet::tcp_socket& player);
    // virtual void onClientDisconnect();
    int client_count = 0;
    int ready_count  = 0;

   private:
    //
    kissnet::tcp_socket server;
    //
    std::list<kissnet::tcp_socket> connections;
    //
    std::atomic<bool> accept_connections = false;
    //
    std::vector<std::thread> workers;
    //
    void listen(kissnet::tcp_socket& socket);

    // The server debugger
    std::shared_ptr<NetServerDebugger> net_server_debugger = nullptr;

    // The number of clients

    // Packet Queue
    std::queue<std::unique_ptr<Packet>> packets;
  };
} // namespace bcw::networking
