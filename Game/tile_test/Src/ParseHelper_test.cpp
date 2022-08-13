/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include <vector>

#include <gtest/gtest.h>

#include "Mock/XMLMock.h"

#include "ParseHelper.h"

using namespace testing;

namespace FA {

namespace Tile {

TEST(ParseHelper, ParseTileSetShouldSucceed)
{
    ParseHelper<XMLElementMock, XMLError> h;
    TileSetData expected{"tsname", 16, 80, 3, 0};
    XMLElementMock mock;

    EXPECT_CALL(mock, QueryStringAttribute(StrEq("name"), An<const char**>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.name_.c_str()), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilewidth"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileWidth_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tileheight"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileHeight_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilecount"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileCount_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("columns"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.columns_), Return(XML_SUCCESS)));

    TileSetData result;
    EXPECT_EQ(true, (h.ParseTileSet(&mock, result)));
    EXPECT_EQ(expected, result);
}

TEST(ParseHelper, ParseTileSetShouldFailDueToWrongAttributeName)
{
    ParseHelper<XMLElementMock, XMLError> h;
    TileSetData expected{"tsname", 16, {}, 3, 0};
    XMLElementMock mock;

    EXPECT_CALL(mock, QueryStringAttribute(StrEq("name"), An<const char**>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.name_.c_str()), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilewidth"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileWidth_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tileheight"), An<unsigned int*>()))
        .WillOnce(Return(XML_WRONG_ATTRIBUTE_TYPE));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilecount"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileCount_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("columns"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.columns_), Return(XML_SUCCESS)));

    TileSetData result;
    EXPECT_EQ(false, (h.ParseTileSet(&mock, result)));
    EXPECT_EQ(expected, result);
}

TEST(ParseHelper, ParseTileSetShouldFailDueToNoAttribute)
{
    ParseHelper<XMLElementMock, XMLError> h;
    TileSetData expected{{}, 16, 0, 3, 0};
    XMLElementMock mock;

    EXPECT_CALL(mock, QueryStringAttribute(StrEq("name"), An<const char**>())).WillOnce(Return(XML_NO_ATTRIBUTE));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilewidth"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileWidth_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tileheight"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileHeight_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("tilecount"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.tileCount_), Return(XML_SUCCESS)));
    EXPECT_CALL(mock, QueryAttribute(StrEq("columns"), An<unsigned int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(expected.columns_), Return(XML_SUCCESS)));

    TileSetData result;
    EXPECT_EQ(false, (h.ParseTileSet(&mock, result)));
    EXPECT_EQ(expected, result);
}

}  // namespace Tile

}  // namespace FA
