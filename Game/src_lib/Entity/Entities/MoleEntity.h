/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEntity.h"

namespace FA {

namespace Entity {

class MoleEntity : public BasicEntity
{
public:
    MoleEntity(EntityId id, MessageBus& messageBus, std::unique_ptr<BasicSprite> sprite, FaceDirection faceDir,
               float velocity);
    virtual ~MoleEntity();

    virtual std::string Name() const override { return "MoleEntity"; }
    virtual void OnCreate() override;
    virtual void OnDestroy() override;
};

}  // namespace Entity

}  // namespace FA
