/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "TileMap.h"

#include "FrameData.h"
#include "GridTileSet.h"
#include "ImageTileSet.h"
#include "Logging.h"

namespace FA {

namespace Tile {

TileMap::TileMap(SheetManager& sheetManager, unsigned int scale)
    : sheetManager_(sheetManager)
    , scale_(scale)
{}

TileMap::~TileMap() = default;

void TileMap::Load(const TileMapData& tileMapData)
{
    tileMapData_ = tileMapData;
    LoadTileSets();
}

void TileMap::Setup()
{
    LOG_INFO("Setup tile map");
    SetupLayers();
    SetupObjectGroups();
}

std::unique_ptr<BasicTileSet> TileMap::CreateTileSet(const TileMapData::TileSet& tileSet) const
{
    if (!tileSet.tiles_.empty()) {
        return std::make_unique<ImageTileSet>(tileSet, sheetManager_);
    }
    else {
        return std::make_unique<GridTileSet>(tileSet, sheetManager_);
    }
}

void TileMap::LoadTileSets()
{
    for (const auto& tileSet : tileMapData_.tileSets_) {
        auto s = CreateTileSet(tileSet);
        s->Load();
        auto firstGid = tileSet.firstGid_;
        tileSets_[firstGid] = std::move(s);
    }
}

void TileMap::SetupLayers()
{
    auto nCols = tileMapData_.mapProperties_.width_;
    auto tileWidth = tileMapData_.mapProperties_.tileWidth_;
    auto tileHeight = tileMapData_.mapProperties_.tileHeight_;

    for (const auto& layer : tileMapData_.layers_) {
        int inx = 0;
        auto layerName = layer.name_;
        for (auto it = layer.tileIds_.begin(); layer.tileIds_.end() != it; ++it, ++inx) {
            auto tileId = *it;
            if (tileId == 0) continue;
            auto frameData = GetFrameData(tileId);
            TileMap::TileData tileData;
            unsigned int x = (inx % nCols) * tileWidth;
            unsigned int y = (inx / nCols) * tileHeight;
            if (frameData.frame_.rect_.height > tileHeight) {
                y += tileHeight;
                y -= frameData.frame_.rect_.height;
            }
            tileData.position_ = {x * scale_, y * scale_};
            tileData.scale_ = scale_;
            tileData.frameData_ = frameData;
            layers_[layerName].push_back(tileData);
        }
    }
}

void TileMap::SetupObjectGroups()
{
    for (const auto& group : tileMapData_.objectGroups_) {
        auto groupName = group.name_;
        std::vector<TileMap::ObjectData> objectDatas;
        for (const auto& object : group.objects_) {
            TileMap::ObjectData objectData;
            objectData.position_ = {object.x_ * scale_, object.y_ * scale_};
            objectData.scale_ = scale_;
            objectData.typeStr_ = object.typeStr_;
            objectData.faceDirStr_ = object.properties_.at("FaceDirection");
            objectDatas.push_back(objectData);
        }
        objectGroups_[groupName] = objectDatas;
    }
}

const std::vector<TileMap::TileData>& TileMap::GetLayer(const std::string& name) const
{
    return layers_.at(name);
}

const std::vector<TileMap::ObjectData> TileMap::GetObjectGroup(const std::string& name) const
{
    return objectGroups_.at(name);
}

sf::Vector2u TileMap::GetSize() const
{
    auto nCols = tileMapData_.mapProperties_.width_;
    auto nRows = tileMapData_.mapProperties_.height_;
    auto tileWidth = tileMapData_.mapProperties_.tileWidth_;
    auto tileHeight = tileMapData_.mapProperties_.tileHeight_;

    return {nCols * tileWidth * scale_, nRows * tileHeight * scale_};
}

FrameData TileMap::GetFrameData(int id)
{
    auto it = tileSets_.lower_bound(id);

    if (it != tileSets_.end()) {
        auto firstGid = it->first;
        return it->second->GetFrameData(id - firstGid);
    }
    else {
        LOG_ERROR("Id ", id, "not found");
        return {};
    }
}

}  // namespace Tile

}  // namespace FA
