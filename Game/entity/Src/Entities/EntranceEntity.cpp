/*
 *	Copyright (C) 2024 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "EntranceEntity.h"

#include "Animation/Animation.h"
#include "Animator/Animator.h"
#include "PropertyConverter.h"
#include "RectangleShape.h"
#include "Resource/ColliderData.h"
#include "Resource/ColliderFrame.h"
#include "State.h"

namespace FA {

namespace Entity {

const std::string EntranceEntity::str = "Entrance";

EntranceEntity::EntranceEntity(EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> service)
    : BasicEntity(id, data, std::move(service))
{}

EntranceEntity::~EntranceEntity() = default;

void EntranceEntity::RegisterProperties()
{
    propertyStore_.Register("ExitId", 0);
}

void EntranceEntity::ReadProperties(const std::unordered_map<std::string, std::string>& properties)
{
    for (const auto& p : properties) {
        if (p.first == "ExitId") {
            int entranceId = ToValue<int>(p.second);
            propertyStore_.Set(p.first, entranceId);
        }
    }
}

void EntranceEntity::RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                    const Shared::EntityData& data)
{
    const sf::Vector2i size{1, 1};
    const Shared::ColliderData colliderData(size);
    const std::vector<Shared::ColliderData> idleColliders{colliderData};
    auto colliderAnimation = service_->CreateColliderAnimation(idleColliders);
    auto rect = idleState->RegisterCollider(Shape::ColliderType::Wall);
    auto colliderAnimator = std::make_shared<Animator<Shared::ColliderFrame>>(*rect, colliderAnimation);
    idleState->RegisterColliderAnimator(colliderAnimator);
}

}  // namespace Entity

}  // namespace FA
