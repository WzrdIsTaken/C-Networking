// Pain

#pragma once

#include "ASGEGameLib/Driver.hpp"

#include "ASGEGameLib/Components/Animator/Animator.hpp"

#include "ASGEGameLib/Components/TileMap/PathFinder.hpp"
#include "gameobjects/actors/units/Unit.hpp"

class GigaScene // NOLINT don't care
  : public bcw::Scene
  , public bcw::IClickHandler
{
 public:
  GigaScene()           = default;
  ~GigaScene() override = default;

  // --- Global Stuff ---

  // loading resources on threads ):
  enum GlobalGameState
  {
    LOBBY,
    GAME
  };
  GlobalGameState global_game_state = LOBBY;

  void start() override;
  void update(const ASGE::GameTime& game_time) override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

  // Returns global_game_state
  GlobalGameState getGlobalGameState() const;

 private:
  // The client (has to be stored as a scene to stop circular dependency in Client.hpp :/)
  std::shared_ptr<bcw::Scene> client = nullptr;

  // --- Lobby ---

 public:
  // I know these functions could be combined, but I think this is clearer... ↓↓↓
  enum LobbyPlayer
  {
    LEFT,
    RIGHT
  };

  // Called when a player connects to the lobby
  void lobbyOnPlayerConnect(LobbyPlayer player, int player_count);
  // Called when a player disconnects to the lobby
  void lobbyOnPlayerDisconnect(LobbyPlayer player, int player_count);
  // Called when a player readys up
  void lobbyOnPlayerReady(LobbyPlayer player, int ready_count);
  // Called when a player readys down
  void lobbyOnPlayerUnready(LobbyPlayer player, int ready_count);
  // Called when the game wants to start
  void lobbyOnGameStart();

 private:
  // Updates the ready text
  void lobbyUpdateReadyText();

  // Background
  std::unique_ptr<ASGE::Sprite> lobby_background_image;

  // The cool cardholders
  std::unique_ptr<ASGE::Sprite> lobby_cardholder_left;
  std::unique_ptr<ASGE::Sprite> lobby_cardholder_right;

  // The player cards
  std::unique_ptr<ASGE::Sprite> lobby_player_card_left_not_ready;
  std::unique_ptr<ASGE::Sprite> lobby_player_card_right_not_ready;

  std::unique_ptr<ASGE::Sprite> lobby_player_card_left_ready;
  std::unique_ptr<ASGE::Sprite> lobby_player_card_right_ready;

  bool lobby_left_player_ready  = false;
  bool lobby_right_player_ready = false;

  // Loading circles
  std::unique_ptr<ASGE::Sprite> lobby_loading_circle_left;
  std::unique_ptr<ASGE::Sprite> lobby_loading_circle_right;

  // The loading thingy animator
  std::unique_ptr<bcw::Animator> lobby_loading_circle_left_animator;
  std::unique_ptr<bcw::Animator> lobby_loading_circle_right_animator;

  // Menu text
  ASGE::Text lobby_return_to_menu_text;
  ASGE::Text lobby_play_text;

  // Don't worry, these are only for local rendering in updateReadyText()!
  int lobby_connected_players      = 0;
  int lobby_ready_players          = 0; // ): not anymore
  bool lobby_local_player_is_ready = false;

  // --- Game ---

 public:
  // The game state. Used to block / allow inputs from happening
  enum GameGameState
  {
    LOADING,
    DEFAULT,
    UNIT_ACTION_OCCURRING,
    GAME_OVER
  };

  // Setups up the game with the servers values
  void gameSetupGame(Unit::Team _player_team, Unit::Team first_move_team);

  // Moves a unit along a specified path
  void gameOnUnitMove(const std::shared_ptr<Unit>& unit, const std::vector<bcw::Vector2>& path);
  // Causes one unit to deal damage to another
  void gameOnUnitAttack(const std::shared_ptr<Unit>& attacker, const std::shared_ptr<Unit>& target);

  // Ends the game and sets the match result text
  enum GameMatchResult
  {
    WIN,
    LOSE,
    DISCONNECT
  };
  void gameOnGameOver(GameMatchResult match_result);

  // Sets the game state
  void gameSetGameState(GameGameState _game_state);
  // Removes a unit from the units vector
  void gameKillUnit(const std::shared_ptr<Unit>& unit);

  // Gets the units vector
  std::vector<std::shared_ptr<Unit>> gameGetUnits() const;

 private:
  // Makes a... nice... square around the unit
  void gameSelectUnit(const std::shared_ptr<Unit>& unit);
  // Calculates if the move is valid
  void gameMoveUnit(std::shared_ptr<Unit> unit, const bcw::Vector2& mouse_pos);
  // Calculates if a unit can make a move
  bool gameCalculateIfValidDistance(
    const std::shared_ptr<Unit>& unit, const bcw::Vector2& target_position, int check_distance);

  // Updates the players image (idk what to call it!) if its their turn or not
  void gameUpdatePlayerTurnSprite(Unit::Team whoseTurn);

  // Controls whose turn it is
  void gameChangeTurn(const std::shared_ptr<Unit>& unit);

  // Used to make sure the user can't do things when they aren't supposed to
  // Should be stored on the server?
  GameGameState game_game_state = LOADING;

  // What side the player is on
  // Should be stored on the server? Needs to be decided on the server
  Unit::Team game_player_team = Unit::DEMON;

  // Simple background sprite
  std::unique_ptr<ASGE::Sprite> game_background_image;

  // Player avatars
  std::unique_ptr<ASGE::Sprite> game_player_left_avatar_not_turn;
  std::unique_ptr<ASGE::Sprite> game_player_right_avatar_not_turn;

  std::unique_ptr<ASGE::Sprite> game_player_left_avatar_turn;
  std::unique_ptr<ASGE::Sprite> game_player_right_avatar_turn;

  // Turn logic
  bool game_left_player_turn  = false;
  bool game_right_player_turn = false;

  int move_count;
  const int MAX_MOVES = 3;
  bool my_turn        = false; // very bot

  // Game over logic (these guys are basically consts... they shouldn't be changed!)
  int NUMBER_OF_DEMONS;
  int NUMBER_OF_ORCS;

  // The tilemap
  std::unique_ptr<bcw::TileMap> game_tile_map;

  // All the sprites in the tilemap
  std::vector<std::unique_ptr<ASGE::Sprite>> game_tile_map_sprites;

  // The units
  std::vector<std::shared_ptr<Unit>> game_units;

  // The pathfinder object
  std::unique_ptr<bcw::PathFinder> game_path_finder;

  // Which unit is currently being acted upon
  std::shared_ptr<Unit> game_currently_selected_unit;

  // Simple text
  ASGE::Text game_match_result_text;
  ASGE::Text game_return_to_menu_text;
};
