/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicState.h"

namespace FA {

namespace Entity {

class AttackState : public BasicState
{
public:
    AttackState(StateMachine& stateMachine, const AnimationFactory& animationFactory, StateData& stateData);
    virtual ~AttackState();

    virtual void Update(float deltaTime) override;
    virtual std::string Name() const override { return "AttackState"; }
    virtual void Enter() override;

    virtual void OnStartMove(MoveDirection moveDir, FaceDirection faceDir) override;

private:
    std::unique_ptr<Animation> animation_ = nullptr;
};

}  // namespace Entity

}  // namespace FA