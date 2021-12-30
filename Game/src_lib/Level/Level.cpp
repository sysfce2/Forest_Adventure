/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "Level.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Entity/PropertyData.h"
#include "Folder.h"
#include "Logging.h"
#include "Resource/SheetData.h"
#include "Resource/SheetId.h"
#include "Tile/TileMapReader.h"

namespace FA {

namespace {

const std::vector<SheetData> textureSheets = {
    {SheetId::HeroWalkSide, "hero/walk/hero-walk-side.png", {6, 1}},
    {SheetId::HeroWalkFront, "hero/walk/hero-walk-front.png", {6, 1}},
    {SheetId::HeroWalkBack, "hero/walk/hero-back-walk.png", {6, 1}},
    {SheetId::HeroAttackSide, "hero/attack/hero-attack-side.png", {3, 1}},
    {SheetId::HeroAttackFront, "hero/attack/hero-attack-front.png", {3, 1}},
    {SheetId::HeroAttackBack, "hero/attack/hero-attack-back.png", {3, 1}},
    {SheetId::HeroAttackWeaponSide, "hero/attack-weapon/hero-attack-side-weapon.png", {3, 1}},
    {SheetId::HeroAttackWeaponFront, "hero/attack-weapon/hero-attack-front-weapon.png", {3, 1}},
    {SheetId::HeroAttackWeaponBack, "hero/attack-weapon/hero-attack-back-weapon.png", {3, 1}},
    {SheetId::HeroIdleSide, "hero/idle/hero-idle-side.png", {1, 1}},
    {SheetId::HeroIdleFront, "hero/idle/hero-idle-front.png", {1, 1}},
    {SheetId::HeroIdleBack, "hero/idle/hero-idle-back.png", {1, 1}},
    {SheetId::MoleWalkSide, "mole/walk/mole-walk-side.png", {6, 1}},
    {SheetId::MoleWalkFront, "mole/walk/mole-walk-front.png", {6, 1}},
    {SheetId::MoleWalkBack, "mole/walk/mole-walk-back.png", {6, 1}},
    {SheetId::MoleIdleSide, "mole/idle/mole-idle-side.png", {1, 1}},
    {SheetId::MoleIdleFront, "mole/idle/mole-idle-front.png", {1, 1}},
    {SheetId::MoleIdleBack, "mole/idle/mole-idle-back.png", {1, 1}}};

}  // namespace

Level::Level(MessageBus &messageBus, TextureManager &textureManager)
    : factory_(messageBus)
    , entityManager_(factory_)
    , tileMap_(textureManager, scale_)
    , sheetManager_(textureManager)
{}

Level::~Level() = default;

void Level::Load()
{
    auto path = GetAssetsPath() + "/map/test.tmx";
    Tile::TileMapReader tileMapReader;
    auto tileMapData = tileMapReader.Parse(path);
    tileMap_.Create(tileMapData);

    for (const auto& sheetData : textureSheets) {
        sheetManager_.LoadSheet(sheetData);
    }
}

void Level::Create(CameraManager &cameraManager)
{
    LOG_INFO_ENTER_FUNC();

    LOG_INFO("Create entities");
    for (const auto &objectData : tileMap_.GetObjectGroup("Object Layer 1")) {
        Entity::PropertyData data;
        data.position_ = static_cast<sf::Vector2f>(objectData.position_);
        data.faceDir_ = objectData.faceDir_;
        data.velocity_ = 120.0;
        data.scale_ = static_cast<float>(tileMap_.GetScale());
        entityManager_.Create(objectData.type_, data, cameraManager, sheetManager_);
    }

    entityManager_.Init();

    LOG_INFO("Create background texture");
    backgroundTexture_.create(tileMap_.GetSize().x, tileMap_.GetSize().y);
    for (const auto &tile : tileMap_.GetLayer("Ground Layer 1")) {
        backgroundTexture_.draw(tile);
    }
    for (const auto &tile : tileMap_.GetLayer("Ground Layer 2")) {
        backgroundTexture_.draw(tile);
    }
    backgroundTexture_.display();
    backgroundSprite_.setTexture(backgroundTexture_.getTexture());

    fringeLayer_ = tileMap_.GetLayer("Fringe Layer");
    LOG_INFO_EXIT_FUNC();
}

void Level::Update(float deltaTime)
{
    entityManager_.Update(deltaTime);
}

void Level::Draw(sf::RenderTarget &renderTarget)
{
    renderTarget.draw(backgroundSprite_);
    entityManager_.DrawTo(renderTarget);
    for (const auto &tile : fringeLayer_) {
        renderTarget.draw(tile);
    }
}

void Level::EnableInput(bool enable)
{
    entityManager_.EnableInput(enable);
}

sf::Vector2u Level::GetMapSize() const
{
    return tileMap_.GetSize();
}

}  // namespace FA
