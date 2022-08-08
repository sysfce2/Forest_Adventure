/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include <gtest/gtest.h>

#include "Mock/XMLMock.h"

#include "TsxParser.h"

using namespace testing;

namespace FA {

namespace Tile {

TEST(TestTsxParser, ParseShouldFailDueToError)
{
    XMLDocumentMock docMock;
    FA::Tile::TsxParser<XMLDocumentMock, XMLElementMock> p;

    EXPECT_CALL(docMock, LoadFile(_));
    EXPECT_CALL(docMock, Error()).WillOnce(Return(true));

    EXPECT_EQ(false, (p.Parse("t", &docMock)));
}

TEST(TestTsxParser, ParseShouldSucceed)
{
    XMLDocumentMock docMock;
    XMLElementMock tileSetElementMock;
    FA::Tile::TsxParser<XMLDocumentMock, XMLElementMock> p;

    EXPECT_CALL(docMock, LoadFile(_));
    EXPECT_CALL(docMock, Error()).WillOnce(Return(false));
    EXPECT_CALL(docMock, FirstChildElement(StrEq("tileset"))).WillOnce(Return(&tileSetElementMock));

    EXPECT_CALL(tileSetElementMock, Attribute(StrEq("name"))).WillOnce(Return("a"));
    EXPECT_CALL(tileSetElementMock, QueryAttribute(_, An<int*>()))
        .Times(4)
        .WillRepeatedly(DoAll(SetArgPointee<1>(3), Return(XML_SUCCESS)));

    XMLElementMock tileElementMock;
    EXPECT_CALL(tileSetElementMock, FirstChildElement(_)).WillOnce(Return(&tileElementMock));
    EXPECT_CALL(tileElementMock, QueryAttribute(StrEq("id"), An<int*>()))
        .WillOnce(DoAll(SetArgPointee<1>(10), Return(XML_SUCCESS)));
    XMLElementMock imageElementMock;
    EXPECT_CALL(tileElementMock, FirstChildElement(StrEq("image"))).WillOnce(Return(&imageElementMock));

    // ParseImageElement
    EXPECT_CALL(imageElementMock, Attribute(StrEq("source"))).WillOnce(Return("source_name"));
    EXPECT_CALL(imageElementMock, QueryAttribute(_, An<unsigned int*>()))
        .Times(2)
        .WillRepeatedly(DoAll(SetArgPointee<1>(15), Return(XML_SUCCESS)));

    EXPECT_CALL(tileElementMock, FirstChildElement(StrEq("animation"))).WillOnce(Return(nullptr));

    EXPECT_CALL(tileElementMock, NextSiblingElement(_)).WillOnce(Return(nullptr));

    EXPECT_EQ(true, (p.Parse("t", &docMock)));
}

}  // namespace Tile

}  // namespace FA