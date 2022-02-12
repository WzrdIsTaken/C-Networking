// Created by Ben on 03/02/2022

#include "gameobjects/actors/units/Archer.hpp"
#include "gameobjects/actors/units/Knight.hpp"
#include "gameobjects/actors/units/Mage.hpp"

#include "GameScene.hpp"
#include "MainMenuScene.hpp"

#include "gameobjects/network/Client.hpp"

void GameScene::start()
{
  // Background
  background_image = bcw::Driver::getRenderer()->createUniqueSprite();
  background_image->loadTexture("/data/ui/GameBackground.png");

  // Avatars
  player_left_avatar = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_left_avatar.get(), "/data/ui/DinoNotTurn.png", bcw::Vector2(0, 0), 7);

  player_right_avatar = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_right_avatar.get(), "/data/ui/WizardNotTurn.png", bcw::Vector2(1084, 0), 7);
  player_right_avatar->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

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

  tile_map = std::make_unique<bcw::TileMap>(
    bcw::TileMap::LoadMode::FROM_STRING, example_map, example_tile_size, bcw::Vector2(140, 175));
  tile_map_sprites = tile_map->drawMap(example_sprites, example_tile_scale);

  path_finder = std::make_unique<bcw::PathFinder>(tile_map.get());

  // Units (perhaps get the start positions from the server?)
  bcw::Vector2 demon_knight_start_pos = tile_map->tileToWorld(bcw::Vector2Int(2, 5));
  bcw::Vector2 demon_archer_start_pos = tile_map->tileToWorld(bcw::Vector2Int(1, 4));
  bcw::Vector2 demon_mage_start_pos   = tile_map->tileToWorld(bcw::Vector2Int(1, 6));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Knight>(this->shared_from_this(), demon_knight_start_pos, Unit::Team::DEMON))));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Archer>(this->shared_from_this(), demon_archer_start_pos, Unit::Team::DEMON))));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Mage>(this->shared_from_this(), demon_mage_start_pos, Unit::Team::DEMON))));

  bcw::Vector2 orc_knight_start_pos = tile_map->tileToWorld(bcw::Vector2Int(17, 5));
  bcw::Vector2 orc_archer_start_pos = tile_map->tileToWorld(bcw::Vector2Int(18, 4));
  bcw::Vector2 orc_mage_start_pos   = tile_map->tileToWorld(bcw::Vector2Int(18, 6));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Knight>(this->shared_from_this(), orc_knight_start_pos, Unit::Team::ORC))));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Archer>(this->shared_from_this(), orc_archer_start_pos, Unit::Team::ORC))));

  units.emplace_back(std::dynamic_pointer_cast<Unit>(bcw::Driver::getSceneHandler()->addScene(
    std::make_shared<Mage>(this->shared_from_this(), orc_mage_start_pos, Unit::Team::ORC))));

  // Set game state, then we are good to go!
  game_state = LOADING;
  game_state = DEFAULT;                // imagine this is some cool server callback
  setupGame(Unit::DEMON, Unit::DEMON); // this too
}

void GameScene::render(const ASGE::GameTime& /*game_time*/)
{
  if (game_state == GameState::GAME_OVER)
  {
    bcw::Driver::getRenderer()->renderText(match_result_text);
    bcw::Driver::getRenderer()->renderText(return_to_menu_text);
  }

  bcw::Driver::getRenderer()->renderSprite(*background_image);

  for (auto& sprite : tile_map_sprites)
  {
    bcw::Driver::getRenderer()->renderSprite(*sprite);
  }

  bcw::Driver::getRenderer()->renderSprite(*player_left_avatar);
  bcw::Driver::getRenderer()->renderSprite(*player_right_avatar);
}

void GameScene::onMouse(const ASGE::ClickEvent* mouse_event)
{
  if (game_state == GameState::DEFAULT)
  {
    for (const auto& unit : units)
    {
      if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *unit->getSprite()))
      {
        selectUnit(unit);
        return;
      }
    }

    for (const auto& tile_map_sprite : tile_map_sprites)
    {
      if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *tile_map_sprite))
      {
        bcw::Vector2 mouse_pos = bcw::Vector2(
          static_cast<float>(mouse_event->xpos), static_cast<float>(mouse_event->ypos));
        moveUnit(currently_selected_unit, mouse_pos);
        return;
      }
    }

    // In case the user clicks outside the map and wants to deselect their unit
    if (bcw::Driver::getInputHandler()->checkClick(mouse_event, *background_image))
    {
      currently_selected_unit->select(false);
      currently_selected_unit = nullptr;
    }
  }

  else if (game_state == GameState::GAME_OVER)
  {
    if (bcw::Driver::getInputHandler()->checkClick(mouse_event, return_to_menu_text))
    {
      bcw::Driver::getSceneHandler()->removeScene(
        bcw::Driver::getSceneHandler()->findScene<Client>());
      bcw::Driver::getSceneHandler()->changeScene(std::make_shared<MainMenuScene>());
    }
  }
}

void GameScene::setupGame(const Unit::Team _player_team, const Unit::Team first_move_team) // NOLINT
{
  player_team = _player_team;
  // bool managing whos turn it is set on server
  // other stuff like map here?

  updatePlayerTurnSprite(first_move_team);
}

void GameScene::setGameState(GameScene::GameState _game_state)
{
  game_state = _game_state;
}

void GameScene::killUnit(const std::shared_ptr<Unit>& unit)
{
  auto it = std::find(units.begin(), units.end(), unit);
  if (it != units.end())
  {
    units.erase(it);
  }
}

void GameScene::onUnitMove(const std::shared_ptr<Unit>& unit, const std::vector<bcw::Vector2>& path)
{
  unit->sendPath(path);
}

void GameScene::onUnitAttack(
  const std::shared_ptr<Unit>& attacker, const std::shared_ptr<Unit>& target)
{
  attacker->attack(target);
}

void GameScene::selectUnit(const std::shared_ptr<Unit>& unit)
{
  if (currently_selected_unit != nullptr)
  {
    currently_selected_unit->select(false);
  }

  // The player clicks the currently_selected_unit, so they want to deselect it
  if (currently_selected_unit == unit)
  {
    currently_selected_unit = nullptr;
  }
  // No unit is selected or the player wants to change the unit they are selecting
  else if (unit->getTeam() == player_team)
  {
    currently_selected_unit = unit;
    currently_selected_unit->select(true);
  }
  // The player clicks an enemy unit, so they want to attack it
  else if (currently_selected_unit != nullptr && unit->getTeam() != player_team)
  {
    // cool serverside attack calc? just this for debugging:
    bcw::Vector2 target_sprite_pos =
      bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());

    if (!calculateIfValidDistance( // NOLINT dude my code was nice looking until this formatter
          currently_selected_unit,
          target_sprite_pos,
          unit->getUnitStats().attack_range))
      return;

    // onUnitAttack(currently_selected_unit, unit);
    std::dynamic_pointer_cast<Client>(client)->sendAttack(
      currently_selected_unit->getTeam(),
      currently_selected_unit->getType(),
      unit->getTeam(),
      unit->getType());
    currently_selected_unit = nullptr;
  }
}

// Cannot be a reference!
// Because if it's a reference, unit = currently_selected_unit so when it's set to null so will
// unit!
void GameScene::moveUnit(const std::shared_ptr<Unit> unit, const bcw::Vector2& mouse_pos) // NOLINT
{
  if (unit == nullptr) // NOLINT one-line | or it would be if it wasn't for this formatter ;_;
    return;

  currently_selected_unit->select(false);
  currently_selected_unit = nullptr;

  if (!calculateIfValidDistance(unit, mouse_pos, unit->getUnitStats().move_distance)) // NOLINT
    return;

  // cool serverside move calc? just this for debugging:
  bcw::Vector2 unit_pos = bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());
  std::vector<bcw::Vector2> path = path_finder->findPath(unit_pos, mouse_pos);

  // game_state = UNIT_ACTION_OCCURRING; todo uncomment this?
  std::dynamic_pointer_cast<Client>(client)->sendMove(
    unit->getType(), unit->getType(), path.end()->x, path.end()->y);
}

bool GameScene::calculateIfValidDistance(
  const std::shared_ptr<Unit>& unit, const bcw::Vector2& target_position, const int check_distance)
{
  bcw::Vector2 unit_pos = bcw::Vector2(unit->getSprite()->xPos(), unit->getSprite()->yPos());
  bcw::Vector2Int unit_tile_cords    = tile_map->worldToTile(unit_pos);
  bcw::Vector2Int target_tile_coords = tile_map->worldToTile(target_position);

  return unit_tile_cords.distance(target_tile_coords) < check_distance;
}

void GameScene::updatePlayerTurnSprite(Unit::Team whoseTurn)
{
  if (whoseTurn == Unit::DEMON)
  {
    bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
      player_left_avatar.get(), "/data/ui/DinoTurn.png");
    bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
      player_right_avatar.get(), "/data/ui/WizardNotTurn.png");
  }
  else
  {
    bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
      player_left_avatar.get(), "/data/ui/DinoNotTurn.png");
    bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
      player_right_avatar.get(), "/data/ui/WizardTurn.png");
  }
}

void GameScene::onGameOver(const GameScene::MatchResult match_result)
{
  const int FONT_BIG   = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 75);
  const int FONT_SMALL = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 30);

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

  match_result_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_BIG))
    .setString(match_result_str)
    .setZOrder(5)
    .setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
      bcw::RenderHandler::X, match_result_text, 360));

  return_to_menu_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Return To Menu")
    .setZOrder(5)
    .setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
      bcw::RenderHandler::X, return_to_menu_text, 440));

  game_state = GameState::GAME_OVER;
}

std::vector<std::shared_ptr<Unit>> GameScene::getUnits() const
{
  return units;
}
