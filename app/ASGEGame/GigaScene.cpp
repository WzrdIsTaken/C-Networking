// Threading is pain

#include "GigaScene.hpp"

#include "MainMenuScene.hpp"

#include "gameobjects/actors/units/Archer.hpp"
#include "gameobjects/actors/units/Knight.hpp"
#include "gameobjects/actors/units/Mage.hpp"
#include "gameobjects/network/Client.hpp"

// --- General ---

void GigaScene::start()
{
  // region Lobby

  // Background
  lobby_background_image = bcw::Driver::getRenderer()->createUniqueSprite();
  lobby_background_image->loadTexture("/data/ui/MenuBackground.png");

  // Images
  lobby_cardholder_left = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_cardholder_left.get(), "/data/ui/CardStand.png", bcw::Vector2(324, 500), 3);

  lobby_cardholder_right = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_cardholder_right.get(), "/data/ui/CardStand.png", bcw::Vector2(884, 500), 3);

  std::vector<std::string> loading_animation = { "/data/ui/loading_thingy/loading_thing_f0.png",
                                                 "/data/ui/loading_thingy/loading_thing_f1.png",
                                                 "/data/ui/loading_thingy/loading_thing_f2.png",
                                                 "/data/ui/loading_thingy/loading_thing_f3.png" };
  std::map<std::string, std::vector<std::string>> animations = { { "Default", loading_animation } };

  lobby_loading_circle_left = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_loading_circle_left.get(), loading_animation.front(), bcw::Vector2(340, 400), 3);

  lobby_loading_circle_right = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_loading_circle_right.get(), loading_animation.front(), bcw::Vector2(900, 400), 3);

  lobby_loading_circle_left_animator =
    std::make_unique<bcw::Animator>(lobby_loading_circle_left.get(), animations, 0.12F);
  lobby_loading_circle_right_animator =
    std::make_unique<bcw::Animator>(lobby_loading_circle_right.get(), animations, 0.12F);

  lobby_player_card_left_not_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_player_card_left_not_ready.get(), "/data/ui/DinoNotReady.png", bcw::Vector2(324, 410), 3);
  lobby_player_card_left_not_ready->opacity(0);

  lobby_player_card_right_not_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_player_card_right_not_ready.get(),
    "/data/ui/WizardNotReady.png",
    bcw::Vector2(884, 410),
    3);
  lobby_player_card_right_not_ready->setFlipFlags(ASGE::Sprite::FLIP_X);
  lobby_player_card_right_not_ready->opacity(0);

  lobby_player_card_left_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_player_card_left_ready.get(), "/data/ui/DinoReady.png", bcw::Vector2(324, 410), 3);

  lobby_player_card_right_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    lobby_player_card_right_ready.get(), "/data/ui/WizardReady.png", bcw::Vector2(884, 410), 3);
  lobby_player_card_right_ready->setFlipFlags(ASGE::Sprite::FLIP_X);

  // Text
  const int FONT_BIG   = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 75);
  const int FONT_SMALL = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 30);

  lobby_return_to_menu_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Return to Menu")
    .setPosition(bcw::Vector2(25, 75));

  lobby_play_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_BIG))
    .setString("Waiting For Players...")
    .setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
      bcw::RenderHandler::X, lobby_play_text, 650));

  // Client
  client = bcw::Driver::getSceneHandler()->findScene<Client>();
  if (client == nullptr)
  {
    client = bcw::Driver::getSceneHandler()->addScene(std::make_shared<Client>());
  }
  std::dynamic_pointer_cast<Client>(client)->setScene(Client::GIGA, this->shared_from_this());

  // endregion

  // region Game

  // Background
  game_background_image = bcw::Driver::getRenderer()->createUniqueSprite();
  game_background_image->loadTexture("/data/ui/GameBackground.png");

  // Avatars
  game_player_left_avatar_not_turn = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    game_player_left_avatar_not_turn.get(), "/data/ui/DinoNotTurn.png", bcw::Vector2(0, 0), 7);

  game_player_right_avatar_not_turn = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    game_player_right_avatar_not_turn.get(), "/data/ui/WizardNotTurn.png", bcw::Vector2(1084, 0), 7);
  game_player_right_avatar_not_turn->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

  game_player_left_avatar_turn = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    game_player_left_avatar_turn.get(), "/data/ui/DinoTurn.png", bcw::Vector2(0, 0), 7);

  game_player_right_avatar_turn = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    game_player_right_avatar_turn.get(), "/data/ui/WizardTurn.png", bcw::Vector2(1084, 0), 7);
  game_player_right_avatar_turn->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

  // Text
  game_match_result_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_BIG))
    .setString("yoo")
    .setZOrder(5);

  game_return_to_menu_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Return To Menu")
    .setZOrder(5)
    .setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
      bcw::RenderHandler::X, game_return_to_menu_text, 440));

  // Client
  client = bcw::Driver::getSceneHandler()->findScene<Client>();
  if (client != nullptr)
  {
    std::dynamic_pointer_cast<Client>(client)->setScene(Client::GAME, this->shared_from_this());
  }

  // Map
  std::string example_map = "####################\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "#..................#\n"
                            "####################";

  std::map<bcw::TileMap::TileData::TileType, std::string> example_sprites{
    { bcw::TileMap::TileData::TileType::FLOOR, "/data/world/floor_1.png" },
    { bcw::TileMap::TileData::TileType::WALL, "/data/world/wall_1.png" }
  };

  float example_tile_size  = 50;
  float example_tile_scale = 3.2F;

  game_tile_map = std::make_unique<bcw::TileMap>(
    bcw::TileMap::LoadMode::FROM_STRING, example_map, example_tile_size, bcw::Vector2(140, 175));
  game_tile_map_sprites = game_tile_map->drawMap(example_sprites, example_tile_scale);

  game_path_finder = std::make_unique<bcw::PathFinder>(game_tile_map.get());

  // Units (perhaps get the start positions from the server?)
  bcw::Vector2 demon_knight_start_pos = game_tile_map->tileToWorld(bcw::Vector2Int(2, 5));
  bcw::Vector2 demon_archer_start_pos = game_tile_map->tileToWorld(bcw::Vector2Int(1, 4));
  bcw::Vector2 demon_mage_start_pos   = game_tile_map->tileToWorld(bcw::Vector2Int(1, 6));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Knight>(this->shared_from_this(), demon_knight_start_pos, Unit::Team::DEMON))));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Archer>(this->shared_from_this(), demon_archer_start_pos, Unit::Team::DEMON))));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Mage>(this->shared_from_this(), demon_mage_start_pos, Unit::Team::DEMON))));

  bcw::Vector2 orc_knight_start_pos = game_tile_map->tileToWorld(bcw::Vector2Int(17, 5));
  bcw::Vector2 orc_archer_start_pos = game_tile_map->tileToWorld(bcw::Vector2Int(18, 4));
  bcw::Vector2 orc_mage_start_pos   = game_tile_map->tileToWorld(bcw::Vector2Int(18, 6));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Knight>(this->shared_from_this(), orc_knight_start_pos, Unit::Team::ORC))));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Archer>(this->shared_from_this(), orc_archer_start_pos, Unit::Team::ORC))));

  game_units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Mage>(this->shared_from_this(), orc_mage_start_pos, Unit::Team::ORC))));

  // Because everything is one scene now...
  for (auto& unit : game_units)
  {
    unit->setActiveState(false);
  }

  // Set game state, then we are good to go!
  game_game_state = LOADING;

  // endregion
}

void GigaScene::update(const ASGE::GameTime& game_time)
{
  switch (global_game_state)
  {
    case LOBBY:
      lobby_loading_circle_left_animator->update(game_time);
      lobby_loading_circle_right_animator->update(game_time);
      break;
    case GAME:
      break;
  }
}

void GigaScene::render(const ASGE::GameTime& /*game_time*/)
{
  switch (global_game_state)
  {
    case LOBBY:
      bcw::Driver::getRenderer()->renderSprite(*lobby_background_image);

      bcw::Driver::getRenderer()->renderSprite(*lobby_cardholder_left);
      bcw::Driver::getRenderer()->renderSprite(*lobby_cardholder_right);

      bcw::Driver::getRenderer()->renderSprite(*lobby_loading_circle_left);
      bcw::Driver::getRenderer()->renderSprite(*lobby_loading_circle_right);

      bcw::Driver::getRenderer()->renderSprite(
        !lobby_left_player_ready ? *lobby_player_card_left_not_ready
                                 : *lobby_player_card_left_ready);
      bcw::Driver::getRenderer()->renderSprite(
        !lobby_right_player_ready ? *lobby_player_card_right_not_ready
                                  : *lobby_player_card_right_ready);

      bcw::Driver::getRenderer()->renderText(lobby_return_to_menu_text);
      bcw::Driver::getRenderer()->renderText(lobby_play_text);
      break;
    case GAME:
      if (game_game_state == GigaScene::GameGameState::GAME_OVER)
      {
        bcw::Driver::getRenderer()->renderText(game_match_result_text);
        bcw::Driver::getRenderer()->renderText(game_return_to_menu_text);
      }

      bcw::Driver::getRenderer()->renderSprite(*game_background_image);

      for (auto& sprite : game_tile_map_sprites)
      {
        bcw::Driver::getRenderer()->renderSprite(*sprite);
      }

      bcw::Driver::getRenderer()->renderSprite(
        !game_left_player_turn ? *game_player_left_avatar_not_turn : *game_player_left_avatar_turn);
      bcw::Driver::getRenderer()->renderSprite(
        !game_right_player_turn ? *game_player_right_avatar_not_turn
                                : *game_player_right_avatar_turn);
      break;
  }
}

void GigaScene::onMouse(const ASGE::ClickEvent* mouse_event) // NOLINT :)
{
  switch (global_game_state)
  {
    case LOBBY:
      if (bcw::Driver::getInputHandler()->checkClick(mouse_event, lobby_return_to_menu_text))
      {
        bcw::Driver::getSceneHandler()->removeScene(
          bcw::Driver::getSceneHandler()->findScene<Client>());

        std::shared_ptr<MainMenuScene> main_menu = std::make_shared<MainMenuScene>();
        bcw::Driver::getSceneHandler()->changeScene(main_menu);
      }
      else if (bcw::Driver::getInputHandler()->checkClick(mouse_event, lobby_play_text))
      {
        lobby_local_player_is_ready = !lobby_local_player_is_ready;
        std::dynamic_pointer_cast<Client>(client)->sendReady(lobby_local_player_is_ready);
      }
      break;
    case GAME:
      if (game_game_state == GigaScene::GameGameState::DEFAULT && my_turn)
      {
        for (const auto& unit : game_units)
        {
          if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *unit->getSprite()))
          {
            gameSelectUnit(unit);
            return;
          }
        }

        for (const auto& tile_map_sprite : game_tile_map_sprites)
        {
          if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *tile_map_sprite))
          {
            bcw::Vector2 mouse_pos = bcw::Vector2(
              static_cast<float>(mouse_event->xpos), static_cast<float>(mouse_event->ypos));
            gameMoveUnit(game_currently_selected_unit, mouse_pos);
            return;
          }
        }

        // In case the user clicks outside the map and wants to deselect their unit
        if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *game_background_image))
        {
          game_currently_selected_unit->select(false);
          game_currently_selected_unit = nullptr;
        }
      }

      else if (game_game_state == GigaScene::GameGameState::GAME_OVER)
      {
        if (bcw::Driver::getInputHandler()->checkClick(mouse_event, game_return_to_menu_text))
        {
          bcw::Driver::getSceneHandler()->removeScene(
            bcw::Driver::getSceneHandler()->findScene<Client>());

          bcw::Driver::getSceneHandler()->changeScene(std::make_shared<MainMenuScene>());
        }
      }
      break;
  }
}

GigaScene::GlobalGameState GigaScene::getGlobalGameState() const
{
  return global_game_state;
}

// --- Lobby ---

void GigaScene::lobbyOnPlayerConnect(const GigaScene::LobbyPlayer player, const int player_count)
{
  if (player == GigaScene::LobbyPlayer::LEFT) // Left
  {
    lobby_player_card_left_not_ready->opacity(1);
    lobby_loading_circle_left->opacity(0);
  }
  else // Right
  {
    lobby_player_card_right_not_ready->opacity(1);
    lobby_loading_circle_right->opacity(0);
  }

  lobby_connected_players = player_count;
  lobbyUpdateReadyText();
}

void GigaScene::lobbyOnPlayerDisconnect(const GigaScene::LobbyPlayer player, const int player_count)
{
  if (player == GigaScene::LobbyPlayer::LEFT) // Left
  {
    lobby_left_player_ready = false;
    lobby_player_card_left_not_ready->opacity(0);
    lobby_loading_circle_left->opacity(1);
  }
  else // Right
  {
    lobby_right_player_ready = false;
    lobby_player_card_right_not_ready->opacity(0);
    lobby_loading_circle_right->opacity(1);
  }

  lobby_connected_players = player_count;
  lobbyUpdateReadyText();

  // onPlayerUnready(player); will be done on the server
}

void GigaScene::lobbyOnPlayerReady(const GigaScene::LobbyPlayer player, const int ready_count)
{
  if (player == GigaScene::LobbyPlayer::LEFT) // Left
  {
    lobby_left_player_ready = true;
  }
  else // Right
  {
    lobby_right_player_ready = true;
  }

  lobby_local_player_is_ready = true;
  lobby_ready_players         = ready_count;
  lobbyUpdateReadyText();
}

void GigaScene::lobbyOnPlayerUnready(const GigaScene::LobbyPlayer player, const int ready_count)
{
  if (player == GigaScene::LobbyPlayer::LEFT) // Left
  {
    lobby_left_player_ready = false;
  }
  else // Right
  {
    lobby_right_player_ready = false;
  }

  lobby_local_player_is_ready = false;
  lobby_ready_players         = ready_count;
  lobbyUpdateReadyText();
}

void GigaScene::lobbyOnGameStart()
{
  global_game_state = GlobalGameState::GAME;

  for (auto& unit : game_units)
  {
    unit->setActiveState(true);
  }

  const PlayerID LOCAL_PLAYER_ID = std::dynamic_pointer_cast<Client>(client)->getPlayerID();
  gameSetupGame(LOCAL_PLAYER_ID == PlayerID::PLAYERONE ? Unit::DEMON : Unit::ORC, Unit::DEMON);
}

void GigaScene::lobbyUpdateReadyText()
{
  if (lobby_connected_players != 0)
  {
    std::string local_ready = lobby_local_player_is_ready ? "Not Ready" : "Ready!";
    std::string total_ready = (" (" + std::to_string(lobby_ready_players) + "/2)");

    lobby_play_text.setString(local_ready + total_ready);
  }
  else
  {
    lobby_play_text.setString("Waiting For Players...");
  }

  lobby_play_text.setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
    bcw::RenderHandler::X, lobby_play_text, lobby_play_text.getPosition().y));
}

// --- Game ---

void GigaScene::gameSetupGame(
  const Unit::Team _player_team, const Unit::Team first_move_team) // NOLINT
{
  game_player_team = _player_team;
  gameUpdatePlayerTurnSprite(first_move_team);

  if (game_player_team == first_move_team)
  {
    my_turn = true;
  }
  game_game_state = DEFAULT;

  for (const auto& unit : game_units)
  {
    switch (unit->getTeam())
    {
      case Unit::DEMON:
        NUMBER_OF_DEMONS++;
        break;
      case Unit::ORC:
        NUMBER_OF_ORCS++;
        break;
    }
  }
}

void GigaScene::gameSetGameState(GigaScene::GameGameState _game_state)
{
  game_game_state = _game_state;
}

void GigaScene::gameKillUnit(const std::shared_ptr<Unit>& unit)
{
  // Remove the unit
  auto it = std::find(game_units.begin(), game_units.end(), unit);
  if (it != game_units.end())
  {
    game_units.erase(it);
  }

  // Check if a player has one
  int demon_count = NUMBER_OF_DEMONS;
  int orc_count   = NUMBER_OF_ORCS;

  for (const auto& cooler_unit : game_units)
  {
    switch (cooler_unit->getTeam())
    {
      case Unit::DEMON:
        demon_count--;
        break;
      case Unit::ORC:
        orc_count--;
        break;
    }
  }

  if (
    (demon_count == NUMBER_OF_DEMONS && game_player_team == Unit::ORC) ||
    (orc_count == NUMBER_OF_ORCS && game_player_team == Unit::DEMON))
  {
    std::dynamic_pointer_cast<Client>(client)->sendWin();
  }
}

void GigaScene::gameOnUnitMove(
  const std::shared_ptr<Unit>& unit, const std::vector<bcw::Vector2>& path)
{
  unit->sendPath(path);
  gameChangeTurn(unit);
}

void GigaScene::gameOnUnitAttack(
  const std::shared_ptr<Unit>& attacker, const std::shared_ptr<Unit>& target)
{
  attacker->attack(target);
  gameChangeTurn(attacker);
}

void GigaScene::gameChangeTurn(const std::shared_ptr<Unit>& unit)
{
  move_count++;
  if (move_count == MAX_MOVES)
  {
    move_count = 0;

    my_turn = unit->getTeam() != game_player_team;
    gameUpdatePlayerTurnSprite(
      my_turn                           ? game_player_team
      : game_player_team == Unit::DEMON ? Unit::ORC
                                        : Unit::DEMON);
  }
}

void GigaScene::gameSelectUnit(const std::shared_ptr<Unit>& unit)
{
  if (game_currently_selected_unit != nullptr)
  {
    game_currently_selected_unit->select(false);
  }

  // The player clicks the currently_selected_unit, so they want to deselect it
  if (game_currently_selected_unit == unit)
  {
    game_currently_selected_unit = nullptr;
  }
  // No unit is selected or the player wants to change the unit they are selecting
  else if (unit->getTeam() == game_player_team)
  {
    game_currently_selected_unit = unit;
    game_currently_selected_unit->select(true);
  }
  // The player clicks an enemy unit, so they want to attack it
  else if (game_currently_selected_unit != nullptr && unit->getTeam() != game_player_team)
  {
    // cool serverside attack calc? just this for debugging:
    bcw::Vector2 target_sprite_pos =
      bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());

    if (!gameCalculateIfValidDistance( // NOLINT dude my code was nice looking until this formatter
          game_currently_selected_unit,
          target_sprite_pos,
          unit->getUnitStats().attack_range))
      return;

    // onUnitAttack(currently_selected_unit, unit);
    std::dynamic_pointer_cast<Client>(client)->sendAttack(
      game_currently_selected_unit->getTeam(),
      game_currently_selected_unit->getType(),
      unit->getTeam(),
      unit->getType());
    game_currently_selected_unit = nullptr;
  }
}

// Cannot be a reference!
// Because if it's a reference, unit = currently_selected_unit so when it's set to null so will
// unit!
void GigaScene::gameMoveUnit(
  const std::shared_ptr<Unit> unit, const bcw::Vector2& mouse_pos) // NOLINT
{
  if (unit == nullptr) // NOLINT one-line | or it would be if it wasn't for this formatter ;_;
    return;

  game_currently_selected_unit->select(false);
  game_currently_selected_unit = nullptr;

  if (!gameCalculateIfValidDistance(unit, mouse_pos, unit->getUnitStats().move_distance)) // NOLINT
    return;

  // cool serverside move calc? just this for debugging:
  // bcw::Vector2 unit_pos = bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());
  // std::vector<bcw::Vector2> path = game_path_finder->findPath(unit_pos, mouse_pos);

  // game_state = UNIT_ACTION_OCCURRING; todo comment this in?

  std::dynamic_pointer_cast<Client>(client)->sendMove(
    unit->getTeam(), unit->getType(), mouse_pos.x, mouse_pos.y); // getType() getType 11/02/2022 ;_;
}

bool GigaScene::gameCalculateIfValidDistance(
  const std::shared_ptr<Unit>& unit, const bcw::Vector2& target_position, const int check_distance)
{
  bcw::Vector2 unit_pos = bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());
  bcw::Vector2Int unit_tile_cords    = game_tile_map->worldToTile(unit_pos);
  bcw::Vector2Int target_tile_coords = game_tile_map->worldToTile(target_position);

  return unit_tile_cords.distance(target_tile_coords) < check_distance;
}

void GigaScene::gameUpdatePlayerTurnSprite(Unit::Team whoseTurn)
{
  if (whoseTurn == Unit::DEMON)
  {
    game_left_player_turn  = true;
    game_right_player_turn = false;
  }
  else
  {
    game_left_player_turn  = false;
    game_right_player_turn = true;
  }
}

void GigaScene::gameOnGameOver(const GigaScene::GameMatchResult match_result)
{
  std::string match_result_str;
  switch (match_result)
  {
    case WIN:
      match_result_str = "You win!";
      break;
    case LOSE:
      match_result_str = "You lose!";
      break;
    case DISCONNECT:
      match_result_str = "Your opponent disconnected ):";
      break;
  }

  game_match_result_text.setString(match_result_str)
    .setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
      bcw::RenderHandler::X, game_match_result_text, 360));
  game_game_state = GameGameState::GAME_OVER;
}

std::vector<std::shared_ptr<Unit>> GigaScene::gameGetUnits() const
{
  return game_units;
}
