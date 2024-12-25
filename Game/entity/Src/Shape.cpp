/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Shape.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Body.h"
#include "RectangleShape.h"
#include "RenderTargetIf.h"
#include "Sprite.h"

namespace FA {

namespace Entity {

Shape::Shape(Body &body)
    : body_(body)
{
#ifdef _DEBUG
    rShape_.setSize({1.0, 1.0});
#endif
}

Shape::~Shape() = default;

void Shape::Enter()
{
    for (auto part : shapeParts_) {
        part->Enter();
    }
    for (auto &sprite : sprites_) {
        sprite->setPosition(body_.position_);
        sprite->setRotation(body_.rotation_);
    }

    for (auto part : colliderParts_) {
        part->Enter();
    }
    for (auto &collider : colliders_) {
        collider->setPosition(body_.position_);
        collider->setRotation(body_.rotation_);
    }

#ifdef _DEBUG
    rShape_.setPosition(body_.position_);
#endif
}

void Shape::Update(float deltaTime)
{
    for (auto &element : shapeElements_) {
        element.animation_->Update(deltaTime);
        element.animation_->ApplyTo(element.sprite_);
    }

    for (auto &element : colliderElements_) {
        element.animation_->Update(deltaTime);
        element.animation_->ApplyTo(element.rect_);
    }

    for (auto &sprite : sprites_) {
        sprite->setPosition(body_.position_);
        sprite->setRotation(body_.rotation_);
    }

    for (auto &collider : colliders_) {
        collider->setPosition(body_.position_);
        collider->setRotation(body_.rotation_);
    }

#ifdef _DEBUG
    rShape_.setPosition(body_.position_);
#endif  // _DEBUG
}

std::shared_ptr<Graphic::SpriteIf> Shape::RegisterMainSprite()
{
    auto sprite = std::make_shared<Graphic::Sprite>();
    mainSprite_ = sprite;
    sprites_.push_back(sprite);

    return sprite;
}

std::shared_ptr<Graphic::RectangleShapeIf> Shape::RegisterMainCollider()
{
    auto rect = std::make_shared<Graphic::RectangleShape>();
    rect->setFillColor(sf::Color::Transparent);
    rect->setOutlineColor(sf::Color::Red);
    rect->setOutlineThickness(1.0f);
    mainCollider_ = rect;
    colliders_.push_back(rect);

    return rect;
}

void Shape::RegisterMainShapePart(std::shared_ptr<AnimationPartIf> part, std::shared_ptr<Graphic::SpriteIf> sprite)
{
    mainShapePart_ = part;
    shapeElements_.push_back({part, sprite});
    shapeParts_.push_back(part);
}

void Shape::RegisterMainColliderPart(std::shared_ptr<AnimationPartIf> part,
                                     std::shared_ptr<Graphic::RectangleShapeIf> rect)
{
    mainColliderPart_ = part;
    colliderElements_.push_back({part, rect});
    colliderParts_.push_back(part);
}

void Shape::RegisterShapePart(std::shared_ptr<AnimationPartIf> part)
{
    shapeParts_.push_back(part);
}

void Shape::RegisterColliderPart(std::shared_ptr<AnimationPartIf> part)
{
    colliderParts_.push_back(part);
}

void Shape::DrawTo(Graphic::RenderTargetIf &renderTarget) const
{
    for (auto &sprite : sprites_) {
        renderTarget.draw(*sprite);
    }

#ifdef _DEBUG
    for (auto &collider : colliders_) {
        renderTarget.draw(*collider);
    }

    renderTarget.draw(rShape_);
#endif
}

bool Shape::Intersect(const Shape &otherShape) const
{
    bool intersect = false;

    for (auto &collider : colliders_) {
        for (auto &otherCollider : otherShape.colliders_) {
            intersect |= collider->getGlobalBounds().intersects(otherCollider->getGlobalBounds());
        }
    }

    return intersect;
}

}  // namespace Entity

}  // namespace FA
