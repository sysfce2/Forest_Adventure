/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "StaticEntity.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity/Components/Sprite/BasicSprite.h"
#include "Message/MessageBus.h"

namespace FA {

namespace Entity {

StaticEntity::StaticEntity(MessageBus& messageBus, std::unique_ptr<BasicSprite> sprite, float speed)
    : BasicEntity(messageBus, std::move(sprite), FaceDirection::Down, speed)
{}

StaticEntity::StaticEntity(MessageBus& messageBus, std::unique_ptr<BasicSprite> sprite)
    : BasicEntity(messageBus, std::move(sprite), FaceDirection::Down, 0.0f)
{}

StaticEntity::~StaticEntity() = default;

void StaticEntity::OnCreate()
{}

void StaticEntity::OnDestroy()
{}

}  // namespace Entity

}  // namespace FA