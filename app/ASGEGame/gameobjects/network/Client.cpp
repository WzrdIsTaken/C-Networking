// Created by Ben and James on 10/02/2022

#include "Client.hpp"

#include "../actors/units/Unit.hpp"

void Client::start()
{
  connect("127.0.0.1", 31276);
  setDontDestroyOnLoad(true);
}

void Client::sendReady(const bool /*ready*/)
{
  bcw::networking::Packet ready_packet;
  ready_packet << PacketID::ONPLAYERREADY;
  ready_packet << my_id;
  send(ready_packet);
}

void Client::setScene(const Client::SceneType scene_type, const std::shared_ptr<bcw::Scene>& scene)
{
  switch (scene_type)
  {
    case Client::SceneType::LOBBY:
      lobby_scene = std::dynamic_pointer_cast<LobbyScene>(scene);
      break;
    case Client::SceneType::GAME:
      game_scene = std::dynamic_pointer_cast<GameScene>(scene);
      break;
    case Client::SceneType::GIGA:
      giga_scene = std::dynamic_pointer_cast<GigaScene>(scene);
      break;
  }
}

void Client::onPlayerConnect(int player_count)
{
  if (player_count == 1)
  {
    giga_scene->lobbyOnPlayerConnect(GigaScene::LobbyPlayer::LEFT, player_count);
  }
  else if (player_count == 2)
  {
    giga_scene->lobbyOnPlayerConnect(GigaScene::LobbyPlayer::LEFT, player_count);
    giga_scene->lobbyOnPlayerConnect(GigaScene::LobbyPlayer::RIGHT, player_count);
  }
}

void Client::onPlayerDisconnect(const PlayerID player_id, const int player_count)
{
  if (giga_scene->getGlobalGameState() == GigaScene::GlobalGameState::LOBBY)
  {
    giga_scene->lobbyOnPlayerDisconnect(
      player_id == PlayerID::PLAYERONE ? GigaScene::LobbyPlayer::LEFT
                                       : GigaScene::LobbyPlayer::RIGHT,
      player_count);
  }
  else
  {
    giga_scene->gameOnGameOver(GigaScene::GameMatchResult::DISCONNECT);
  }
}

void Client::onPlayerReady(PlayerID player_id, int ready_count)
{
  GigaScene::LobbyPlayer side = GigaScene::LobbyPlayer::LEFT;
  if (player_id == PlayerID::PLAYERONE)
  {
    side = GigaScene::LobbyPlayer::LEFT;
  }
  else if (player_id == PlayerID::PLAYERTWO)
  {
    side = GigaScene::LobbyPlayer::RIGHT;
  }
  giga_scene->lobbyOnPlayerReady(side, ready_count);
}

void Client::onPlayerUnready(const int ready_count)
{
  // take in player number as well
  int player_num = 1; // todo change this

  giga_scene->lobbyOnPlayerUnready(
    player_num == 1 ? GigaScene::LobbyPlayer::LEFT : GigaScene::LobbyPlayer::RIGHT, ready_count);
}

void Client::onUnitAttack(
  PlayerID /*id*/, int attackerteam, int attackertype, int target_team, int target_type)
{
  for (const auto& attacker : giga_scene->gameGetUnits())
  {
    if (attacker->getTeam() == attackerteam && attacker->getType() == attackertype)
    {
      for (const auto& target : giga_scene->gameGetUnits())
      {
        if (target->getTeam() == target_team && target->getType() == target_type)
        {
          giga_scene->gameOnUnitAttack(attacker, target);
          return;
        }
      }
    }
  }
}

void Client::onPlayerWin(const PlayerID winning_player_id)
{
  giga_scene->gameOnGameOver(
    winning_player_id == my_id ? GigaScene::GameMatchResult::WIN
                               : GigaScene::GameMatchResult::LOSE);
}

void Client::onGameStart()
{
  giga_scene->lobbyOnGameStart();
}

void Client::onUnitMove(PlayerID /*id*/, int team, int type, float xpos, float ypos)
{
  for (const auto& unit : giga_scene->gameGetUnits())
  {
    /* solved B)
  Logging::INFO(
    "Passed in team: " + std::to_string(team) +
    " | Unit team: " + std::to_string(unit->getTeam()));
  Logging::INFO(
    "Passed in type: " + std::to_string(type) +
    " | Unit type: " + std::to_string(unit->getType()));
  Logging::INFO("---");
     */

    if (unit->getTeam() == team && unit->getType() == type) // - 2 ???
    {
      std::vector<bcw::Vector2> path{ bcw::Vector2(xpos, ypos) };
      giga_scene->gameOnUnitMove(unit, path);
      return;
    }
  }
}

void Client::sendMove(int unit_team, int unit_type, float x_pos, float y_pos)
{
  bcw::networking::Packet packet;
  packet << PacketID::ONPLAYERMOVE;
  packet << my_id;
  packet << unit_team;
  packet << unit_type;
  packet << x_pos;
  packet << y_pos;
  send(packet);
}

void Client::sendAttack(int attacker_team, int attacker_type, int target_team, int target_type)
{
  bcw::networking::Packet packet;
  packet << PacketID::ONPLAYERATTACK;
  packet << my_id;
  packet << attacker_team;
  packet << attacker_type;
  packet << target_team;
  packet << target_type;
  send(packet);
}

void Client::sendWin()
{
  bcw::networking::Packet packet;
  packet << PacketID::ONPLAYERWIN;
  packet << my_id;
  send(packet);
}

PlayerID Client::getPlayerID() const
{
  return my_id;
}
