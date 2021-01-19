/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Game.h"

#include <fstream>
#include <memory>

#include <SFML/Config.hpp>
#include <SFML/System/Err.hpp>

#include "Message/BroadcastMessage/CloseWindowMessage.h"
#include "Message/BroadcastMessage/IsKeyPressedMessage.h"
#include "Message/BroadcastMessage/KeyboardPressedMessage.h"
#include "Message/BroadcastMessage/KeyboardReleasedMessage.h"
#include "System/InputSystem.h"
#include "Utils/Logger.h"
#include "Utils/Path.h"
#include "Version.h"

namespace FA {

const sf::Vector2u Game::screen = {1280, 780};
const sf::Vector2u Game::centerScreen = {Game::screen.x / 2, Game::screen.y / 2};

Game::Game()
    : sceneHelper_(messageBus_, "GamePlayScene")
    , level_(messageBus_, textureManager_)
{
    LOG_INFO("Start up ", FA_APP_NAME, " version ", FA_APP_VERSION);
    LOG_INFO("SFML version ", SFML_VERSION_MAJOR, ".", SFML_VERSION_MINOR);

    RedirectSfmlLogEntries();
    InitWindow();
    auto cb = [this](std::shared_ptr<Message> message) { OnMessage(message); };
    messageBus_.AddSubscriber("game",
                              {MessageType::IsKeyPressed, MessageType::KeyboardPressed, MessageType::KeyboardReleased,
                               MessageType::CloseWindow},
                              cb);
}

Game::~Game()
{
    sfmlLogStream_->close();
}

void Game::GameLoop()
{
    LOG_INFO_ENTER_FUNC();
    sf::Clock clock;
    InputSystem inputSystem(messageBus_, window_);

    while (window_.isOpen()) {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();

        inputSystem.Update(deltaTime);
        messageBus_.DispatchMessages();

        level_.Update(deltaTime);

        window_.clear();

        level_.DrawTo(levelLayer_);
        levelLayer_.DrawTo(window_);
#ifdef _DEBUG
        sceneHelper_.DrawTo(sceneHelperLayer_);
        sceneHelperLayer_.DrawTo(window_);
#endif
        window_.display();
    }

    LOG_INFO_EXIT_FUNC();
}

void Game::OnMessage(std::shared_ptr<Message> message)
{
    if (message->GetMessageType() == MessageType::KeyboardPressed) {
        auto m = std::dynamic_pointer_cast<KeyboardPressedMessage>(message);
        auto key = m->GetKey();
        if (key == Keyboard::Key::Escape) {
            window_.close();
        }
    }
    else if (message->GetMessageType() == MessageType::KeyboardReleased) {
    }
    else if (message->GetMessageType() == MessageType::IsKeyPressed) {
    }
    else if (message->GetMessageType() == MessageType::CloseWindow) {
        window_.close();
    }
    else {
        // cant happened
    }
}

void Game::RedirectSfmlLogEntries()
{
    sfmlLogStream_ = std::make_unique<std::ofstream>();
    const std::string sfmlLog = "sfml-log.txt";
    sfmlLogStream_->open(GetExePath() + "\\/" + sfmlLog);

    if (sfmlLogStream_->is_open()) {
        LOG_INFO("Redirecting sfml log entries to ", sfmlLog);
        (*sfmlLogStream_) << "SFML version " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
        sf::err().rdbuf(sfmlLogStream_->rdbuf());
    }
}

void Game::InitWindow()
{
    LOG_INFO("Create main window");
#ifdef _DEBUG
    const std::string title = std::string(FA_APP_NAME) + " v" + FA_APP_VERSION;
    window_.create(sf::VideoMode(screen.x, screen.y), title);
#else
    window_.create(sf::VideoMode::getDesktopMode(), FA_APP_NAME, sf::Style::Fullscreen);
    view_.reset(sf::FloatRect(0.0, 0.0, static_cast<float>(screen.x), static_cast<float>(screen.y)));
    window_.setView(view_);
#endif
    window_.setFramerateLimit(120);
}

}  // namespace FA
