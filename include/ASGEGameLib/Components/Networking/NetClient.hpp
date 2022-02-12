// Created by huxy on 23/02/2020
// Updated for use in bcw by James on 04/02/2022

#pragma once

#include <kissnet.hpp>

#include "../../Driver.hpp"
#include "Packet.hpp"
#include <string>
#include <thread>
#include <vector>

// The base class of a TCP client that connects to the server.
// Has a bunch of virtual callbacks that can be overridden to implement custom functionality.

namespace bcw::networking
{
  class NetClient
  {
   public:
    NetClient() = default;
    virtual ~NetClient();

    NetClient(const NetClient&) = delete;
    NetClient& operator=(const NetClient&) = delete;

    void connect(const std::string& server_ip, unsigned short server_port);
    void update(const ASGE::GameTime& game_time);

    void listen(kissnet::tcp_socket& csocket);
    void read(Packet& data);

    void send(Packet& packet) { socket.send(&packet.data()[0], packet.length()); }

   protected:
    virtual void onPlayerConnect(int player_count);
    virtual void onPlayerDisconnect(PlayerID player_id, int player_count);
    virtual void onPlayerReady(PlayerID player_id, int ready_count);
    virtual void onPlayerUnready(int ready_count);
    virtual void onGameStart();
    virtual void onUnitMove(PlayerID id, int team, int type, float xpos, float ypos);
    virtual void
    onUnitAttack(PlayerID id, int attackerteam, int attackertype, int target_team, int target_type);
    virtual void onPlayerWin(PlayerID winning_player_id);

    // Identifier
    PlayerID my_id;

   private:
    // packet queue
    std::queue<std::unique_ptr<Packet>> packets;
    //
    kissnet::tcp_socket socket;
    //
    std::vector<std::thread> workers;
  };
} // namespace bcw::networking
