/*
 *	Copyright (C) 2023 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "SfmlFwd.h"

namespace FA {

namespace Entity {

class BasicSprite
{
public:
    virtual ~BasicSprite() = default;

    virtual void Enter() = 0;
    virtual void Update(float deltaTime) = 0;

    virtual void SetPosition(const sf::Vector2f &position) = 0;
    virtual void SetRotation(float rot) = 0;
    virtual void DrawTo(sf::RenderTarget &renderTarget) = 0;
};

}  // namespace Entity

}  // namespace FA
