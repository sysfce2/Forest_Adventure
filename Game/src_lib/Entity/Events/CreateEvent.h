/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#pragma once

#include "BasicEvent.h"

#include "Entity/PropertyData.h"

namespace FA {

namespace Entity {

struct CreateEvent : public BasicEvent
{
    CreateEvent(const PropertyData &data)
        : data_(data)
    {}

    virtual EventType GetEventType() const { return EventType::Create; }

    PropertyData data_;
};

}  // namespace Entity

}  // namespace FA