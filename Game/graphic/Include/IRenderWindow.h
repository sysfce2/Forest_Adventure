/*
 *	Copyright (C) 2023 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <string>

#include "IRenderTarget.h"

namespace sf {

class VideoMode;
class Event;
class Color;
class View;
typedef unsigned int Uint32;

}  // namespace sf

namespace FA {

namespace Graphic {

class IRenderWindow : public IRenderTarget
{
public:
    virtual ~IRenderWindow() override = default;
    virtual void display() = 0;
    virtual bool pollEvent(sf::Event &event) = 0;
    virtual void create(sf::VideoMode mode, const std::string &title) = 0;
    virtual void create(sf::VideoMode mode, const std::string &title, sf::Uint32 style) = 0;
    virtual void close() = 0;
    virtual void setFramerateLimit(unsigned int limit) = 0;
    virtual void clear() = 0;
    virtual void clear(const sf::Color &color) = 0;
    virtual void setView(const sf::View &view) = 0;
};

}  // namespace Graphic

}  // namespace FA
