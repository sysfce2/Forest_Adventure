/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include <unordered_map>

#include "BasicEntity.h"

#include "Enum/MoveDirection.h"

namespace FA {

namespace Entity {

class PlayerEntity : public BasicEntity
{
public:
    static const std::string str;

    PlayerEntity(EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> service);
    virtual ~PlayerEntity();

    virtual EntityType Type() const override { return EntityType::Player; }
    virtual LayerType GetLayer() const override { return LayerType::Ground; }
    virtual bool IsStatic() const override { return false; }
    virtual bool IsSolid() const override { return false; }

protected:
    virtual std::vector<Shared::MessageType> Messages() const override;

private:
    virtual void RegisterProperties() override;
    virtual void ReadProperties(const std::unordered_map<std::string, std::string>& properties) override;
    virtual void RegisterStates(std::shared_ptr<State> idleState, std::shared_ptr<State> deadState,
                                const Shared::EntityData& data) override;
    virtual void OnInit() override;
    virtual void OnMessage(std::shared_ptr<Shared::Message> msg) override;
    virtual void OnBeginDie() override;

    void OnBeginMove(MoveDirection moveDirection);
    void OnUpdateMove(const sf::Vector2f& delta);
    void OnShoot();
    void OnCollision(const EntityIf& collisionEntity);

    void DefineIdleState(std::shared_ptr<State> state);
    void DefineMoveState(std::shared_ptr<State> state);
    void DefineDoorMoveState(std::shared_ptr<State> state);
    void DefineAttackState(std::shared_ptr<State> state);
    void DefineAttackWeaponState(std::shared_ptr<State> state);

private:
    unsigned int coins_{0};
};

}  // namespace Entity

}  // namespace FA
