/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "BasicEffect.h"

#include "RectangleShape.h"

namespace FA {

namespace Shared {

class FadeEffect : public BasicEffect
{
public:
    FadeEffect(const sf::Vector2f& position, const sf::Vector2f& size, float seconds);

    virtual void DrawTo(Graphic::IRenderTarget& renderTarget) const override;
    virtual void Update(float deltaTime) override;

private:
    sf::Clock clock_;
    Graphic::RectangleShape fadeRect_;
    int startAlpha_ = 0;
    int endAlpha_ = 255;
    sf::Time targetTime_;
    sf::Time currentTime_;
};

}  // namespace Shared

}  // namespace FA
