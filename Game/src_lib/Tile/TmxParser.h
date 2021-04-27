/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <string>
#include <vector>

namespace tinyxml2 {

class XMLElement;

}  // namespace tinyxml2

namespace FA {

namespace Tile {

class TmxParser
{
public:
    struct Map
    {
        std::string renderOrder_{};
        float width_{};
        float height_{};
        int tileWidth_{};
        int tileHeight_{};
    };

    struct TileSet
    {
        int firstGid_{};
        std::string source_{};
    };

    struct Layer
    {
        int id_{};
        std::string name_{};
        int width_{};
        int height_{};
        std::vector<int> tileIds_{};
    };

    Map map_;
    std::vector<TileSet> tileSets_;
    std::vector<Layer> layers_;

public:
    TmxParser(const std::string& fileName);
    ~TmxParser();
    bool Load();

private:
    std::string fileName_;

private:
    void ParseMapElement(tinyxml2::XMLElement* mapElement);
    void ParseTileSetElement(tinyxml2::XMLElement* tileSetElement, TileSet& tileSet);
    void ParseLayerElement(tinyxml2::XMLElement* layerElement, Layer& layer);
    void ParseDataElement(tinyxml2::XMLElement* dataElement, std::vector<int>& ids);
};

}  // namespace Tile

}  // namespace FA
