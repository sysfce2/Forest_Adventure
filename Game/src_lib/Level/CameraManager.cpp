/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "CameraManager.h"

#include "Camera.h"

namespace FA {

CameraManager::CameraManager(const sf::Vector2u& renderTargetSize, const sf::Vector2u& mapSize)
    : renderTargetSize_(renderTargetSize)
    , mapSize_(mapSize)
{}

CameraManager::~CameraManager() = default;

void CameraManager::Track(const sf::Vector2f& trackingPoint)
{
    camera_ = std::make_unique<Camera>(trackingPoint, renderTargetSize_, mapSize_);
}

void CameraManager::Update()
{
    if (camera_) camera_->UpdatePosition();
}

sf::View CameraManager::GetView() const
{
    if (camera_) return camera_->GetView();

    return {};
}

}  // namespace FA
