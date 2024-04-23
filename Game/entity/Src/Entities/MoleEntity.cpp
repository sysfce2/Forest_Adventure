/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "MoleEntity.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Abilities/MoveAbility.h"
#include "Constant/Entity.h"
#include "Events/CollisionEvent.h"
#include "Events/DeadEvent.h"
#include "PropertyConverter.h"
#include "PropertyData.h"
#include "Resource/SheetId.h"
#include "ShapeParts/AnimationPart.h"
#include "State.h"

namespace FA {

namespace Entity {

namespace {

using namespace Shared::SheetId;

const std::vector<Shared::ImageData> idleLeft{{MoleIdleSide, {0, 0}, true}};
const std::vector<Shared::ImageData> idleRight{{MoleIdleSide, {0, 0}}};
const std::vector<Shared::ImageData> idleFront{{MoleIdleFront, {0, 0}}};
const std::vector<Shared::ImageData> idleBack{{MoleIdleBack, {0, 0}}};
const std::vector<Shared::ImageData> moveLeft{{MoleWalkSide, {0, 0}, true},
                                              {MoleWalkSide, {1, 0}, true},
                                              {MoleWalkSide, {2, 0}, true},
                                              {MoleWalkSide, {3, 0}, true}};
const std::vector<Shared::ImageData> moveRight{
    {MoleWalkSide, {0, 0}}, {MoleWalkSide, {1, 0}}, {MoleWalkSide, {2, 0}}, {MoleWalkSide, {3, 0}}};
const std::vector<Shared::ImageData> moveDown{
    {MoleWalkFront, {0, 0}}, {MoleWalkFront, {1, 0}}, {MoleWalkFront, {2, 0}}, {MoleWalkFront, {3, 0}}};
const std::vector<Shared::ImageData> moveUp{
    {MoleWalkBack, {0, 0}}, {MoleWalkBack, {1, 0}}, {MoleWalkBack, {2, 0}}, {MoleWalkBack, {3, 0}}};

const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>> idleData{{FaceDirection::Left, idleLeft},
                                                                                 {FaceDirection::Right, idleRight},
                                                                                 {FaceDirection::Down, idleFront},
                                                                                 {FaceDirection::Up, idleBack}};
const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>> moveData{{FaceDirection::Left, moveLeft},
                                                                                 {FaceDirection::Right, moveRight},
                                                                                 {FaceDirection::Down, moveDown},
                                                                                 {FaceDirection::Up, moveUp}};

const std::vector<Shared::ImageData> collision{{Death, {0, 0}}, {Death, {1, 0}}, {Death, {2, 0}},
                                               {Death, {3, 0}}, {Death, {4, 0}}, {Death, {5, 0}}};

FaceDirection MoveDirToFaceDir(MoveDirection moveDirection)
{
    FaceDirection faceDir = FaceDirection::Undefined;

    if (moveDirection == MoveDirection::Down)
        faceDir = FaceDirection::Down;
    else if (moveDirection == MoveDirection::Up)
        faceDir = FaceDirection::Up;
    else if (moveDirection == MoveDirection::Left)
        faceDir = FaceDirection::Left;
    else if (moveDirection == MoveDirection::Right)
        faceDir = FaceDirection::Right;

    return faceDir;
}

}  // namespace

const std::string MoleEntity::str = "Mole";

MoleEntity::MoleEntity(EntityId id, const PropertyData& data, const Shared::MapData& mapData,
                       const EntityService& service)
    : BasicEntity(id, data, mapData, service)
{}

MoleEntity::~MoleEntity() = default;

void MoleEntity::OnBeginMove(MoveDirection moveDirection)
{
    FaceDirection faceDir = MoveDirToFaceDir(moveDirection);
    propertyStore_.Set("FaceDirection", faceDir);
}

void MoleEntity::OnUpdateMove(const sf::Vector2f& delta)
{
    body_.position_ += delta;
}

void MoleEntity::RegisterProperties()
{
    propertyStore_.Register("FaceDirection", FaceDirection::Down);
}

void MoleEntity::ReadProperties(const std::unordered_map<std::string, std::string>& properties)
{
    for (const auto& p : properties) {
        if (p.first == "FaceDirection") {
            FaceDirection dir = ToValue<FaceDirection>(p.second);
            propertyStore_.Set(p.first, dir);
        }
    }
}

void MoleEntity::RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                const PropertyData& data)
{
    DefineIdleState(idleState);
    auto moveState = RegisterState(StateType::Move);
    DefineMoveState(moveState);
    auto collisionState = RegisterState(StateType::Collision);
    DefineCollisionState(collisionState);
}

void MoleEntity::DefineIdleState(std::shared_ptr<State> state)
{
    auto part = MakePart(idleData);
    state->RegisterShapePart(part);
    state->RegisterEventCB(EventType::StartMove,
                           [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Move, event); });
    state->RegisterEventCB(EventType::Collision, [this](std::shared_ptr<BasicEvent> event) {
        auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
        if (service_.GetType(collisionEvent->id_) == EntityType::Arrow) {
            ChangeStateTo(StateType::Collision, event);
        }
    });
    state->RegisterIgnoreEvents({EventType::StopMove});
}

void MoleEntity::DefineMoveState(std::shared_ptr<State> state)
{
    auto move = std::make_shared<MoveAbility>(
        Constant::stdVelocity, [this](MoveDirection d) { OnBeginMove(d); },
        [this](const sf::Vector2f& d) { OnUpdateMove(d); });
    auto part = MakePart(moveData);
    state->RegisterAbility(move);
    state->RegisterShapePart(part);
    state->RegisterEventCB(EventType::StopMove,
                           [this](std::shared_ptr<BasicEvent> event) { ChangeStateTo(StateType::Idle, event); });
    state->RegisterEventCB(EventType::Collision, [this](std::shared_ptr<BasicEvent> event) {
        auto collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
        if (service_.GetType(collisionEvent->id_) == EntityType::Arrow) {
            ChangeStateTo(StateType::Collision, event);
        }
    });
}

void MoleEntity::DefineCollisionState(std::shared_ptr<State> state)
{
    auto updateCB = [this](const Shared::AnimationSprite& animation) {
        if (animation.IsCompleted()) {
            HandleEvent(std::make_shared<DeadEvent>());
        }
    };
    auto animation = service_.MakeAnimation(collision);
    auto part = AnimationPart::Create(animation);
    part->RegisterUpdateCB(updateCB);
    state->RegisterShapePart(part);
    state->RegisterIgnoreEvents({EventType::Collision});
}

std::shared_ptr<AnimationPartWith<FaceDirection>> MoleEntity::MakePart(
    const std::unordered_map<FaceDirection, std::vector<Shared::ImageData>>& data)
{
    FaceDirection* dir = nullptr;
    propertyStore_.GetPtr<FaceDirection>("FaceDirection", dir);
    auto part = AnimationPartWith<FaceDirection>::Create(*dir);
    for (const auto& entry : data) {
        part->RegisterAnimation(entry.first, service_.MakeAnimation(entry.second));
    }

    return part;
}

}  // namespace Entity

}  // namespace FA
