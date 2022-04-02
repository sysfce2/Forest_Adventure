/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "SpawnManager.h"

#include "Entities/BasicEntity.h"
#include "EntityManager.h"
#include "Enum/MoveDirection.h"
#include "Events/StartMoveEvent.h"
#include "PropertyData.h"

namespace FA {

namespace Entity {

SpawnManager::SpawnManager(EntityManager &entityManager)
    : entityManager_(entityManager)
{}

SpawnManager::~SpawnManager() = default;

void SpawnManager::Update(float deltaTime)
{}

void SpawnManager::Spawn(EntityType entityType, const sf::Vector2f &position, FaceDirection faceDir)
{
    Entity::PropertyData data;
    data.position_ = position;

    auto &entity = entityManager_.CreateEntity(entityType, data);
    auto moveDirection = MoveDirection::None;

    if (faceDir == FaceDirection::Down)
        moveDirection = MoveDirection::Down;
    else if (faceDir == FaceDirection::Up)
        moveDirection = MoveDirection::Up;
    else if (faceDir == FaceDirection::Left)
        moveDirection = MoveDirection::Left;
    else if (faceDir == FaceDirection::Right)
        moveDirection = MoveDirection::Right;

    entity.QueueInitEvents(std::make_shared<StartMoveEvent>(moveDirection, faceDir));
}

}  // namespace Entity

}  // namespace FA
