/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Resource/EntityData.h"
#include "Resource/TextureManager.h"
#include "Resource/TileGraphic.h"

namespace FA {

namespace Shared {

class SheetManager;

}  // namespace Shared

namespace Tile {

class TileMapParser;
struct TileMapData;
struct TileData;

}  // namespace Tile

namespace World {

class TileMap
{
public:
    struct TileData
    {
        sf::Vector2f position_{};
        Shared::TileGraphic graphic_{};
    };

public:
    TileMap(Shared::TextureManager &textureManager, Shared::SheetManager &sheetManager);
    ~TileMap();
    void Load(const std::string &fileName);
    void Setup();
    const std::vector<TileData> GetLayer(const std::string &name) const;
    const std::vector<Shared::EntityData> GetEntityGroup(const std::string &name) const;
    sf::Vector2u GetSize() const;

private:
    Shared::TextureManager &textureManager_;
    Shared::SheetManager &sheetManager_;
    std::unique_ptr<Tile::TileMapData> tileMapData_ = nullptr;
    std::unique_ptr<Tile::TileMapParser> tileMapParser_ = nullptr;
    std::map<std::string, std::vector<TileData>> layers_;
    std::map<std::string, std::vector<Shared::EntityData>> entityGroups_;

private:
    void LoadTileSets();
    void SetupLayers();
    void SetupEntityGroups();
    Tile::TileData LookupTileData(int id);
};

}  // namespace World

}  // namespace FA
