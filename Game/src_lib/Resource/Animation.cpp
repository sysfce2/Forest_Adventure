/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace FA {

Animation::Animation(const std::vector<Frame>& frames, unsigned int defaultIndex, float switchTime,
                     std::function<void(sf::Sprite&)> alignFn)
    : frames_(frames)
    , switchTime_(switchTime)
    , time_(0.0)
    , defaultIndex_(defaultIndex)
    , iFrame_(defaultIndex)
    , isValid_(true)
    , alignFn_(alignFn)
{
    nFrames_ = frames.size();
}

void Animation::ApplyTo(sf::Sprite& sprite)
{
    sprite.setTexture(*frames_[iFrame_].texture_);
    sprite.setTextureRect(frames_[iFrame_].rect_);
    if (alignFn_) alignFn_(sprite);
}

void Animation::Update(float deltaTime)
{
    if (!isStopped_ && nFrames_ > 1) {
        time_ += deltaTime;

        while (time_ >= switchTime_) {
            time_ -= switchTime_;
            ++iFrame_ %= nFrames_;
            isCompleted_ = (iFrame_ == 0);
        }
    }
}

void Animation::Start()
{
    isStopped_ = false;
}

void Animation::Stop()
{
    isStopped_ = true;
    iFrame_ = defaultIndex_;
}

bool Animation::IsCompleted() const
{
    return isCompleted_;
}

bool Animation::IsValid() const
{
    return isValid_;
}

}  // namespace FA
