// Created by Ben on 03/02/2022

#pragma once

#include "ASGEGameLib/Driver.hpp"

#include "ASGEGameLib/Components/TileMap/PathFinder.hpp"
#include "gameobjects/actors/units/Unit.hpp"

class GameScene
  : public bcw::Scene
  , public bcw::IClickHandler
{
 public:
  GameScene()           = default;
  ~GameScene() override = default;

  // The game state. Used to block / allow inputs from happening
  enum GameState
  {
    LOADING,
    DEFAULT,
    UNIT_ACTION_OCCURRING,
    GAME_OVER
  };

  void start() override;
  void render(const ASGE::GameTime& game_time) override;
  void onMouse(const ASGE::ClickEvent* mouse_event) override;

  // Setups up the game with the servers values
  void setupGame(Unit::Team _player_team, Unit::Team first_move_team);

  // Moves a unit along a specified path
  void onUnitMove(const std::shared_ptr<Unit>& unit, const std::vector<bcw::Vector2>& path);
  // Causes one unit to deal damage to another
  void onUnitAttack(const std::shared_ptr<Unit>& attacker, const std::shared_ptr<Unit>& target);

  // Ends the game and sets the match result text
  enum MatchResult
  {
    WIN,
    LOSE,
    DISCONNECT
  };
  void onGameOver(MatchResult match_result);

  // Sets the game state
  void setGameState(GameState _game_state);
  // Removes a unit from the units vector
  void killUnit(const std::shared_ptr<Unit>& unit);

  // Gets the units vector
  std::vector<std::shared_ptr<Unit>> getUnits() const;

 private:
  // Makes a... nice... square around the unit
  void selectUnit(const std::shared_ptr<Unit>& unit);
  // Calculates if the move is valid
  void moveUnit(std::shared_ptr<Unit> unit, const bcw::Vector2& mouse_pos);
  // Calculates if a unit can make a move
  bool calculateIfValidDistance(
    const std::shared_ptr<Unit>& unit, const bcw::Vector2& target_position, int check_distance);

  // Updates the players image (idk what to call it!) if its their turn or not
  void updatePlayerTurnSprite(Unit::Team whoseTurn);

  // Used to make sure the user can't do things when they aren't supposed to
  // Should be stored on the server?
  GameState game_state = LOADING;

  // What side the player is on
  // Should be stored on the server? Needs to be decided on the server
  Unit::Team player_team = Unit::DEMON;

  // Simple background sprite
  std::unique_ptr<ASGE::Sprite> background_image;

  // Player avatars
  std::unique_ptr<ASGE::Sprite> player_left_avatar;
  std::unique_ptr<ASGE::Sprite> player_right_avatar;

  // The tilemap
  std::unique_ptr<bcw::TileMap> tile_map;

  // All the sprites in the tilemap
  std::vector<std::unique_ptr<ASGE::Sprite>> tile_map_sprites;

  // The units
  std::vector<std::shared_ptr<Unit>> units;

  // The pathfinder object
  std::unique_ptr<bcw::PathFinder> path_finder;

  // Which unit is currently being acted upon
  std::shared_ptr<Unit> currently_selected_unit;

  // Client
  std::shared_ptr<bcw::Scene> client;

  // Simple text
  ASGE::Text match_result_text;
  ASGE::Text return_to_menu_text;
};
