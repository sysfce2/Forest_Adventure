/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "BasicScene.h"

namespace FA {

namespace Scene {

BasicScene::BasicScene(Manager &sceneManager, MessageBus &messageBus, TextureManager &textureManager,
                       Manager::Components &components, Manager::Data &data)
    : sceneManager_(sceneManager)
    , messageBus_(messageBus)
    , textureManager_(textureManager)
    , components_(components)
    , data_(data)
{}

BasicScene::~BasicScene() = default;

// TODO: Consider to request for sceneSwitch here, and do actual switch after Update().
// Then Update() can continue to execute code after the request is made.
void BasicScene::SwitchScene(std::unique_ptr<BasicScene> newScene)
{
    sceneManager_.SetScene(std::move(newScene));
}

void BasicScene::OnCloseWindow(std::shared_ptr<Message> message)
{
    data_.isRunning_ = false;
}

bool BasicScene::IsRunning() const
{
    return data_.isRunning_;
}

}  // namespace Scene

}  // namespace FA