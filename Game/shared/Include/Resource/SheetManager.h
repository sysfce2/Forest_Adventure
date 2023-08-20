/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <string>
#include <unordered_map>

#include "Resource/ResourceId.h"
#include "Resource/SpriteSheet.h"
#include "SfmlFwd.h"

namespace FA {

namespace Shared {

struct AnimationData;
struct ImageData;
struct TextureRect;

class SheetManager
{
public:
    void AddSheet(const std::string &name, const SpriteSheet &sheet);
    std::vector<TextureRect> MakeRects(const AnimationData &data) const;
    TextureRect MakeRect(const ImageData &data) const;

private:
    std::unordered_map<std::string, SpriteSheet> sheetMap_;

private:
    SpriteSheet GetSheet(const std::string &name) const;
    TextureRect CreateRect(const SpriteSheet &sheet, const sf::Vector2u position) const;
    std::vector<TextureRect> CreateRects(const SpriteSheet &sheet, const sf::Vector2u start, unsigned int nRects) const;
};

}  // namespace Shared

}  // namespace FA
