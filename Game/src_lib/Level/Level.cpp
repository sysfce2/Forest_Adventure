/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Level.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity/Entity.h"
#include "Message/MessageBus.h"
#include "Misc/TextureManager.h"

namespace FA {

Level::Level(MessageBus& messageBus, TextureManager& textureManager)
    : messageBus_(messageBus)
    , entity_(messageBus, textureManager)
{}

Level::~Level() = default;

void Level::DrawTo(sf::RenderWindow& window)
{
    entity_.Draw(window);
}

void Level::Update(float deltaTime)
{
    entity_.Update(deltaTime);
}

}  // namespace FA
