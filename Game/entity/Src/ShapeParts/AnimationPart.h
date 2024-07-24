/*
 *	Copyright (C) 2022 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Animation/ImageAnimation.h"
#include "BasicShapePart.h"
#include "Logging.h"
#include "Sequence.h"
#include "Sprite.h"

namespace FA {

namespace Entity {

template <class KeyT>
class AnimationPartWith : public BasicShapePart
{
    using SelectFn = std::function<Shared::ImageAnimation *(
        const std::unordered_map<KeyT, std::shared_ptr<Shared::ImageAnimation>> &)>;

public:
    static std::shared_ptr<AnimationPartWith<KeyT>> Create(SelectFn selectFn)
    {
        return std::make_shared<CtorHelper>(selectFn);
    }

    static std::shared_ptr<AnimationPartWith<KeyT>> Create(std::shared_ptr<Shared::ImageAnimation> animation)
    {
        return std::make_shared<CtorHelper>(animation);
    }

    virtual ~AnimationPartWith() = default;

    virtual void Enter() override
    {
        currentAnimation_ = selectFn_(map_);
        if (currentAnimation_ == nullptr) {
            LOG_ERROR("Can not select animation");
        }

        currentAnimation_->Restart();
    }

    virtual void Update(float deltaTime) override
    {
        currentAnimation_->Update(deltaTime);
        updateCB_(*currentAnimation_);
    }

    virtual void DrawTo(Graphic::RenderTargetIf &renderTarget) const { currentAnimation_->DrawTo(renderTarget); }

    virtual void SetPosition(const sf::Vector2f &position) override { currentAnimation_->SetPosition(position); }

    virtual void SetRotation(float angle) override { currentAnimation_->SetRotation(angle); }

    bool Intersects(const BasicShapePart &otherPart)
    {
        auto other = static_cast<const AnimationPartWith<KeyT> &>(otherPart);
        return currentAnimation_->Intersects(*other.currentAnimation_);
    }

    void RegisterAnimation(const KeyT key, std::shared_ptr<Shared::ImageAnimation> animation)
    {
        auto res = map_.emplace(key, animation);
        if (!res.second) {
            LOG_WARN("%s is already inserted", DUMP(key));
        }
    }
    void RegisterUpdateCB(std::function<void(const Shared::ImageAnimation &)> updateCB) { updateCB_ = updateCB; }

private:
    /* Constructor for multiple animation, depending on KeyT */
    AnimationPartWith(SelectFn selectFn)
        : selectFn_(selectFn)
        , updateCB_([](const Shared::ImageAnimation &) {})
    {}

    /* Constructor for singel animation */
    AnimationPartWith(std::shared_ptr<Shared::ImageAnimation> animation)
        : updateCB_([](const Shared::ImageAnimation &) {})
    {
        KeyT defaultKey{};
        selectFn_ = [defaultKey](const std::unordered_map<KeyT, std::shared_ptr<Shared::ImageAnimation>> &animations) {
            bool found = animations.find(defaultKey) != animations.end();
            Shared::ImageAnimation *result = nullptr;

            if (found) {
                result = animations.at(defaultKey).get();
            }

            return result;
        };

        RegisterAnimation(defaultKey, animation);
    }

private:
    struct CtorHelper : public AnimationPartWith<KeyT>
    {
        CtorHelper(SelectFn selectFn)
            : AnimationPartWith<KeyT>(selectFn)
        {}

        CtorHelper(std::shared_ptr<Shared::ImageAnimation> animation)
            : AnimationPartWith<KeyT>(animation)
        {}
    };

    std::unordered_map<KeyT, std::shared_ptr<Shared::ImageAnimation>> map_;
    SelectFn selectFn_;
    Shared::ImageAnimation *currentAnimation_{nullptr};
    std::function<void(const Shared::ImageAnimation &)> updateCB_;
};

using AnimationPart = AnimationPartWith<int>;

}  // namespace Entity

}  // namespace FA
