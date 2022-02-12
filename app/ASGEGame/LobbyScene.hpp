// Created by Ben on 01/02/2022

#pragma once

#include "../include/ASGEGameLib/Driver.hpp"

#include "../include/ASGEGameLib/Components/Animator/Animator.hpp"

// Connects the user to connect to the server and waits for another to join

class LobbyScene
  : public bcw::Scene
  , public bcw::IClickHandler
{
 public:
  LobbyScene()           = default;
  ~LobbyScene() override = default;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

  // I know these functions could be combined, but I think this is clearer... ↓↓↓
  enum Player
  {
    LEFT,
    RIGHT
  };
  // Called when a player connects to the lobby
  void onPlayerConnect(Player player, int player_count);
  // Called when a player disconnects to the lobby
  void onPlayerDisconnect(Player player, int player_count);
  // Called when a player readys up
  void onPlayerReady(Player player, int ready_count);
  // Called when a player readys down
  void onPlayerUnready(Player player, int ready_count);
  // Called when the game wants to start
  void onGameStart();

 private:
  // Updates the ready text
  void updateReadyText();

  // Background
  std::unique_ptr<ASGE::Sprite> background_image;

  // The cool cardholders
  std::unique_ptr<ASGE::Sprite> cardholder_left;
  std::unique_ptr<ASGE::Sprite> cardholder_right;

  // The player cards
  std::unique_ptr<ASGE::Sprite> player_card_left_not_ready;
  std::unique_ptr<ASGE::Sprite> player_card_right_not_ready;

  std::unique_ptr<ASGE::Sprite> player_card_left_ready;
  std::unique_ptr<ASGE::Sprite> player_card_right_ready;

  bool left_player_ready  = false;
  bool right_player_ready = false;

  // Loading circles
  std::unique_ptr<ASGE::Sprite> loading_circle_left;
  std::unique_ptr<ASGE::Sprite> loading_circle_right;

  // The loading thingy animator
  std::unique_ptr<bcw::Animator> loading_circle_left_animator;
  std::unique_ptr<bcw::Animator> loading_circle_right_animator;

  // The client (has to be stored as a scene to stop circular dependency in Client.hpp :/)
  std::shared_ptr<bcw::Scene> client = nullptr;

  // Menu text
  ASGE::Text return_to_menu_text;
  ASGE::Text play_text;

  // Don't worry, these are only for local rendering in updateReadyText()!
  int connected_players      = 0;
  int ready_players          = 0; // ): not anymore
  bool local_player_is_ready = false;
};
