// Created by Ben and James on 10/02/2022

// Created when the user first connects to the server and remains until they exit.
// Works around Jame's server code. I'm not implementing anything new here!
// I think we both had different ideas about how networking would work...
// and this class is the bridge between the two xD

#pragma once

#include <ASGEGameLib/Components/Networking/NetClient.hpp>
#include <ASGEGameLib/Scene.hpp>

#include "../../GameScene.hpp"
#include "../../GigaScene.hpp"
#include "../../LobbyScene.hpp"

class Client
  : public bcw::Scene
  , public bcw::networking::NetClient
{
 public:
  Client()           = default;
  ~Client() override = default;

  void start() override;

  // Set local scene
  enum SceneType
  {
    LOBBY,
    GAME,
    GIGA
  };
  void setScene(SceneType scene_type, const std::shared_ptr<bcw::Scene>& scene);

  // Sending data
  void sendReady(bool ready);
  void sendMove(int unit_team, int unit_type, float x_pos, float y_pos);
  void sendAttack(int attacker_team, int attacker_type, int target_team, int target_type);
  void sendWin();

  // Returns my_id
  PlayerID getPlayerID() const;

 protected:
  // Callbacks
  void onPlayerConnect(int player_count) override;
  void onPlayerDisconnect(PlayerID player_id, int player_count) override;
  void onPlayerReady(PlayerID player_id, int ready_count) override;
  void onPlayerUnready(int ready_count) override;

  void onGameStart() override;
  void onUnitMove(PlayerID id, int team, int type, float xpos, float ypos) override;
  void onUnitAttack(
    PlayerID id, int attackerteam, int attackertype, int target_team, int target_type) override;
  void onPlayerWin(PlayerID winning_player_id) override;

 private:
  std::shared_ptr<LobbyScene> lobby_scene;
  std::shared_ptr<GameScene> game_scene;
  std::shared_ptr<GigaScene> giga_scene;
};
