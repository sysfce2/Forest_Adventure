/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "MoleEntity.h"

#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity/Attributes/FaceDirectionAttribute.h"
#include "Entity/Attributes/TransformAttribute.h"
#include "Entity/Attributes/VelocityAttribute.h"
#include "Entity/Modes/IdleMode.h"
#include "Entity/Modes/MoveMode.h"
#include "Entity/PropertyData.h"
#include "Entity/Shapes/AnimationShape.h"
#include "Entity/Shapes/Shape.h"
#include "Message/MessageBus.h"

namespace FA {

namespace Entity {

namespace {

FrameType ModeTypeToFrameType(ModeType modeType)
{
    switch (modeType) {
        case ModeType::Idle:
            return FrameType::Idle;
        case ModeType::Move:
            return FrameType::Move;
        default:
            return FrameType::Undefined;
    }
}

}  // namespace

MoleEntity::MoleEntity(EntityId id, CameraManager& cameraManager, const AnimationDb& animationDb,
                       MessageBus& messageBus)
    : BasicEntity(id, EntityType::Mole, cameraManager, animationDb, messageBus)
{}

MoleEntity::~MoleEntity() = default;

void MoleEntity::DefineProperties(EntityService& entityService, const PropertyData& data)
{
    auto t = entityService.AddAttribute<TransformAttribute>();
    t->SetPosition(data.position_);
    t->SetScale(data.scale_);
    std::vector<FaceDirection> dirs = {FaceDirection::Down, FaceDirection::Left, FaceDirection::Right,
                                       FaceDirection::Up};
    auto f = entityService.AddAttribute<FaceDirectionAttribute>();
    f->SetAvailableDirections(dirs);
    auto v = entityService.AddAttribute<VelocityAttribute>();
    v->SetVelocity(data.velocity_);
}

void MoleEntity::DefineModes(ModeController& modeController)
{
    auto idleMode = modeController.AddMode<IdleMode>(true);
    idleMode->BindAction(Action::ChangeTo(ModeType::Move), EventType::StartMove);
    idleMode->BindAction(Action::Ignore(), EventType::Collision);

    auto moveMode = modeController.AddMode<MoveMode>();
    moveMode->BindAction(Action::ChangeTo(ModeType::Idle), EventType::StopMove);
}

void MoleEntity::DefineShape(EntityService& entityService, Shape& shape)
{
    auto lookupKeyFunc = [](FrameType frameType, FaceDirection faceDir) {
        std::stringstream ss;
        ss << frameType << "_" << faceDir;
        return ss.str();
    };

    auto a = std::make_shared<AnimationShape>(lookupKeyFunc);
    auto dirs = entityService.GetAttribute<FaceDirectionAttribute>()->GetAvailableDirections();

    for (auto modeType : entityService.GetModeTypes()) {
        auto frameType = ModeTypeToFrameType(modeType);
        if (frameType == FrameType::Undefined) continue;
        for (auto faceDir : dirs) {
            auto animation = entityService.GetAnimation(frameType, faceDir);
            a->AddAnimation(frameType, faceDir, animation);
        }
    }

    shape.AddAnimationShape(a);
}

}  // namespace Entity

}  // namespace FA
