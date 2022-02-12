// Created by Ben on 09/02/2022

#include "UnitRangeHighlight.hpp"

UnitRangeHighlight::UnitRangeHighlight(
  ASGE::Sprite* _target, const float range, const ASGE::Colour& colour) :
  target(_target),
  RANGE(range)
{
  // note to self... explain this magical mystical number of infinite possibilities
  const float MAGIC_NUMBER = range * 70;

  top   = bcw::Driver::getRenderer()->createUniqueSprite();
  bot   = bcw::Driver::getRenderer()->createUniqueSprite();
  left  = bcw::Driver::getRenderer()->createUniqueSprite();
  right = bcw::Driver::getRenderer()->createUniqueSprite();

  std::vector<ASGE::Sprite*> highlight_sprites{ top.get(), bot.get(), left.get(), right.get() };
  for (auto& sprite : highlight_sprites)
  {
    bcw::Driver::getRenderHandler()->setupSprite(sprite, "/data/ui/CoolCube.png");
    sprite->setGlobalZOrder(6);
    sprite->colour(colour);
  }

  top->width(MAGIC_NUMBER);
  bot->width(MAGIC_NUMBER);
  left->height(MAGIC_NUMBER);
  right->height(MAGIC_NUMBER);
}

void UnitRangeHighlight::update(const ASGE::GameTime& /*game_time*/)
{
  if (!show_highlight) // NOLINT one-line
    return;

  // also this one!!
  const float MAGICAL_MYSTICAL_NUMBER = RANGE * 35;

  top->xPos(target->xPos() + target->width() - top->width() / 2);
  top->yPos(target->yPos() + target->height() - MAGICAL_MYSTICAL_NUMBER);

  bot->xPos(target->xPos() + target->width() - bot->width() / 2);
  bot->yPos(target->yPos() + target->height() + MAGICAL_MYSTICAL_NUMBER);

  left->xPos(target->xPos() + target->width() - MAGICAL_MYSTICAL_NUMBER);
  left->yPos(target->yPos() + target->height() - left->height() / 2);

  right->xPos(target->xPos() + target->width() + MAGICAL_MYSTICAL_NUMBER);
  right->yPos(target->yPos() + target->height() - right->height() / 2);
}

void UnitRangeHighlight::render(const ASGE::GameTime& /*game_time*/)
{
  if (!show_highlight) // NOLINT one-line
    return;

  bcw::Driver::getRenderer()->renderSprite(*top);
  bcw::Driver::getRenderer()->renderSprite(*bot);
  bcw::Driver::getRenderer()->renderSprite(*left);
  bcw::Driver::getRenderer()->renderSprite(*right);
}

void UnitRangeHighlight::toggleHighlight(const bool toggle)
{
  show_highlight = toggle;
}
