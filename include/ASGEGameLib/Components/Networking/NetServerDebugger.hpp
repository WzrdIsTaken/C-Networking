// Created by Ben on 28/01/2022

#pragma once

#include <kissnet.hpp>

// The base class of a server debugger that can be used to make networking less painful.
// Virtual methods can be overridden to add specific functionality.

namespace bcw::networking
{
  class NetServerDebugger
  {
   public:
    NetServerDebugger()          = default;
    virtual ~NetServerDebugger() = default;

    [[maybe_unused]] virtual void onServerStartDebug();
    [[maybe_unused]] virtual void onServerEndDebug();

    [[maybe_unused]] virtual void onSocketOpen(const kissnet::tcp_socket& client);
    [[maybe_unused]] virtual void onSocketClose(const kissnet::tcp_socket& client);

    [[maybe_unused]] virtual void
    onClientConnectDebug(const kissnet::tcp_socket& client, int client_count);
    [[maybe_unused]] virtual void
    onClientDisconnectDebug(const kissnet::tcp_socket& client, int client_count);

    // a bunch more methods
  };
} // namespace bcw::networking
