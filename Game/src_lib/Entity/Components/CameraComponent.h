/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicComponent.h"

namespace FA {

namespace Entity {

class CameraComponent : public BasicComponent
{
public:
    CameraComponent(ComponentHandler *owner);

    virtual void Update(float deltaTime) override {}
};

}  // namespace Entity

}  // namespace FA
