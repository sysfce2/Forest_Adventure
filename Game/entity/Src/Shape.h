/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <memory>
#include <vector>

#ifdef _DEBUG
#include "RectangleShape.h"
#endif

#include "ShapeParts/AnimationPartIf.h"

namespace FA {

namespace Graphic {

class RenderTargetIf;
class SpriteIf;
class RectangleShapeIf;

}  // namespace Graphic

namespace Entity {

struct Body;

class Shape
{
public:
    Shape(Body &body);
    ~Shape();

    void RegisterMainSprite(std::shared_ptr<Graphic::SpriteIf> sprite);
    void RegisterMainCollider(std::shared_ptr<Graphic::RectangleShapeIf> rect);
    void RegisterMainShapePart(std::shared_ptr<AnimationPartIf> part, std::shared_ptr<Graphic::SpriteIf> sprite);
    void RegisterMainColliderPart(std::shared_ptr<AnimationPartIf> part,
                                  std::shared_ptr<Graphic::RectangleShapeIf> rect);
    void RegisterShapePart(std::shared_ptr<AnimationPartIf> part);
    void RegisterColliderPart(std::shared_ptr<AnimationPartIf> part);
    void Enter();
    void Update(float deltaTime);
    void DrawTo(Graphic::RenderTargetIf &renderTarget) const;
    bool Intersect(const Shape &shape) const;

private:
    struct AnimationElement
    {
        std::shared_ptr<AnimationPartIf> animation_;
        std::shared_ptr<Graphic::SpriteIf> sprite_;
    };
    struct ColliderElement
    {
        std::shared_ptr<AnimationPartIf> animation_;
        std::shared_ptr<Graphic::RectangleShapeIf> rect_;
    };

    std::shared_ptr<AnimationPartIf> mainShapePart_;
    std::shared_ptr<AnimationPartIf> mainColliderPart_;
    std::shared_ptr<Graphic::SpriteIf> mainSprite_;
    std::shared_ptr<Graphic::RectangleShapeIf> mainCollider_;
    std::vector<std::shared_ptr<AnimationPartIf>> colliderParts_;
    std::vector<std::shared_ptr<AnimationPartIf>> shapeParts_;
    std::vector<std::shared_ptr<Graphic::SpriteIf>> sprites_;
    std::vector<std::shared_ptr<Graphic::RectangleShapeIf>> colliders_;
    std::vector<AnimationElement> shapeElements_;
    std::vector<ColliderElement> colliderElements_;
    Body &body_;
#ifdef _DEBUG
    Graphic::RectangleShape rShape_;
#endif  // _DEBUG
};

}  // namespace Entity

}  // namespace FA
