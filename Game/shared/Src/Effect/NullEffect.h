/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEffect.h"

namespace FA {

namespace Shared {

class NullEffect : public BasicEffect
{
public:
    NullEffect();

    virtual void DrawTo(Graphic::IRenderTarget& renderTarget) const override;
    virtual void Update(float deltaTime) override;
};

}  // namespace Shared

}  // namespace FA
