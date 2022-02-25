/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Message/MessageBus.h"
#include "Resource/TextureManager.h"
#include "Scene/Manager.h"

namespace FA {

class Game
{
public:
    Game();
    ~Game();
    void GameLoop();

private:
    MessageBus messageBus_;
    sf::RenderWindow window_;
    sf::View view_;
    TextureManager textureManager_;
    Scene::Manager sceneManager_;

private:
    void InitWindow();
};

}  // namespace FA
