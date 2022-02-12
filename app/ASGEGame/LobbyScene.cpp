// Created by Ben on 01/02/2022

#include "LobbyScene.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"

#include "gameobjects/network/Client.hpp"

void LobbyScene::start()
{
  // Background
  background_image = bcw::Driver::getRenderer()->createUniqueSprite();
  background_image->loadTexture("/data/ui/MenuBackground.png");

  // Images
  cardholder_left = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    cardholder_left.get(), "/data/ui/CardStand.png", bcw::Vector2(324, 500), 3);

  cardholder_right = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    cardholder_right.get(), "/data/ui/CardStand.png", bcw::Vector2(884, 500), 3);

  std::vector<std::string> loading_animation = { "/data/ui/loading_thingy/loading_thing_f0.png",
                                                 "/data/ui/loading_thingy/loading_thing_f1.png",
                                                 "/data/ui/loading_thingy/loading_thing_f2.png",
                                                 "/data/ui/loading_thingy/loading_thing_f3.png" };
  std::map<std::string, std::vector<std::string>> animations = { { "Default", loading_animation } };

  loading_circle_left = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    loading_circle_left.get(), loading_animation.front(), bcw::Vector2(340, 400), 3);

  loading_circle_right = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    loading_circle_right.get(), loading_animation.front(), bcw::Vector2(900, 400), 3);

  loading_circle_left_animator =
    std::make_unique<bcw::Animator>(loading_circle_left.get(), animations, 0.12F);
  loading_circle_right_animator =
    std::make_unique<bcw::Animator>(loading_circle_right.get(), animations, 0.12F);

  player_card_left_not_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_card_left_not_ready.get(), "/data/ui/DinoNotReady.png", bcw::Vector2(324, 410), 3);
  player_card_left_not_ready->opacity(0);

  player_card_right_not_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_card_right_not_ready.get(), "/data/ui/WizardNotReady.png", bcw::Vector2(884, 410), 3);
  player_card_right_not_ready->setFlipFlags(ASGE::Sprite::FLIP_X);
  player_card_right_not_ready->opacity(0);

  player_card_left_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_card_left_ready.get(), "/data/ui/DinoReady.png", bcw::Vector2(324, 410), 3);
  // player_card_left_ready->opacity(0);

  player_card_right_ready = bcw::Driver::getRenderer()->createUniqueSprite();
  bcw::Driver::getRenderHandler()->setupSprite(
    player_card_right_ready.get(), "/data/ui/WizardReady.png", bcw::Vector2(884, 410), 3);
  player_card_right_ready->setFlipFlags(ASGE::Sprite::FLIP_X);
  // player_card_right_ready->opacity(0);

  // Text
  const int FONT_BIG   = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 75);
  const int FONT_SMALL = bcw::Driver::getRenderer()->loadFont("/data/text/breathe-fire-ii.ttf", 30);

  return_to_menu_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_SMALL))
    .setString("Return to Menu")
    .setPosition(bcw::Vector2(25, 75));

  play_text.setFont(bcw::Driver::getRenderer()->getFont(FONT_BIG))
    .setString("Waiting For Players...")
    .setPosition(
      bcw::Driver::getRenderHandler()->getScreenCentre(bcw::RenderHandler::X, play_text, 650));

  // Client
  client = bcw::Driver::getSceneHandler()->findScene<Client>();
  if (client == nullptr)
  {
    client = bcw::Driver::getSceneHandler()->addScene(std::make_shared<Client>());
  }
  std::dynamic_pointer_cast<Client>(client)->setScene(Client::LOBBY, this->shared_from_this());
}

void LobbyScene::update(const ASGE::GameTime& game_time)
{
  loading_circle_left_animator->update(game_time);
  loading_circle_right_animator->update(game_time);
}

void LobbyScene::render(const ASGE::GameTime& /*game_time*/)
{
  bcw::Driver::getRenderer()->renderSprite(*background_image);

  bcw::Driver::getRenderer()->renderSprite(*cardholder_left);
  bcw::Driver::getRenderer()->renderSprite(*cardholder_right);

  bcw::Driver::getRenderer()->renderSprite(*loading_circle_left);
  bcw::Driver::getRenderer()->renderSprite(*loading_circle_right);

  bcw::Driver::getRenderer()->renderSprite(
    !left_player_ready ? *player_card_left_not_ready : *player_card_left_ready);
  bcw::Driver::getRenderer()->renderSprite(
    !right_player_ready ? *player_card_right_not_ready : *player_card_right_ready);

  bcw::Driver::getRenderer()->renderText(return_to_menu_text);
  bcw::Driver::getRenderer()->renderText(play_text);
}

void LobbyScene::onMouse(const ASGE::ClickEvent* mouse_event)
{
  if (bcw::Driver::getInputHandler()->checkClick(mouse_event, return_to_menu_text))
  {
    bcw::Driver::getSceneHandler()->removeScene(
      bcw::Driver::getSceneHandler()->findScene<Client>());

    std::shared_ptr<MainMenuScene> main_menu = std::make_shared<MainMenuScene>();
    bcw::Driver::getSceneHandler()->changeScene(main_menu);
  }
  else if (bcw::Driver::getInputHandler()->checkClick(mouse_event, play_text))
  {
    local_player_is_ready = !local_player_is_ready;
    std::dynamic_pointer_cast<Client>(client)->sendReady(local_player_is_ready);

    /* ??? hopefully temp
    if (ready_players < 2)
    {
       onGameStart();
    }
     */
  }
}

void LobbyScene::onPlayerConnect(const LobbyScene::Player player, const int player_count)
{
  if (player == Player::LEFT) // Left
  {
    player_card_left_not_ready->opacity(1);
    loading_circle_left->opacity(0);
  }
  else // Right
  {
    player_card_right_not_ready->opacity(1);
    loading_circle_right->opacity(0);
  }

  connected_players = player_count;
  updateReadyText();
}

void LobbyScene::onPlayerDisconnect(const LobbyScene::Player player, const int player_count)
{
  if (player == Player::LEFT) // Left
  {
    left_player_ready = false;
    player_card_left_not_ready->opacity(0);
    loading_circle_left->opacity(1);
  }
  else // Right
  {
    right_player_ready = false;
    player_card_right_not_ready->opacity(0);
    loading_circle_right->opacity(1);
  }

  connected_players = player_count;
  updateReadyText();

  // onPlayerUnready(player); will be done on the server
}

void LobbyScene::onPlayerReady(const LobbyScene::Player player, const int ready_count)
{
  if (player == Player::LEFT) // Left
  {
    left_player_ready = true;
    // bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
    //        player_card_left_not_ready.get(), "/data/ui/DinoReady.png");
  }
  else // Right
  {
    right_player_ready = true;
    // bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
    //        player_card_right_not_ready.get(), "/data/ui/WizardReady.png");
  }

  local_player_is_ready = true;
  ready_players         = ready_count;
  updateReadyText();
}

void LobbyScene::onPlayerUnready(const LobbyScene::Player player, const int ready_count)
{
  if (player == Player::LEFT) // Left
  {
    left_player_ready = false;
    // bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
    //        player_card_left_not_ready.get(), "/data/ui/DinoNotReady.png");
  }
  else // Right
  {
    right_player_ready = false;
    // bcw::Driver::getRenderHandler()->loadSpriteTextureAndMaintainMagFilter(
    //        player_card_right_not_ready.get(), "/data/ui/WizardNotReady.png");
  }

  local_player_is_ready = false;
  ready_players         = ready_count;
  updateReadyText();
}

void LobbyScene::onGameStart()
{
  std::shared_ptr<GameScene> game_scene = std::make_shared<GameScene>();
  bcw::Driver::getSceneHandler()->changeScene(game_scene);
}

void LobbyScene::updateReadyText()
{
  if (connected_players != 0)
  {
    std::string local_ready = local_player_is_ready ? "Not Ready" : "Ready!";
    std::string total_ready = (" (" + std::to_string(ready_players) + "/2)");

    play_text.setString(local_ready + total_ready);
  }
  else
  {
    play_text.setString("Waiting For Players...");
  }

  play_text.setPosition(bcw::Driver::getRenderHandler()->getScreenCentre(
    bcw::RenderHandler::X, play_text, play_text.getPosition().y));
}
