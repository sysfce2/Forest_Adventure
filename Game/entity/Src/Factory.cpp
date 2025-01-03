/*
 *	Copyright (C) 2021 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include "Factory.h"

#include "Entities/ArrowEntity.h"
#include "Entities/CoinEntity.h"
#include "Entities/EntranceEntity.h"
#include "Entities/MoleEntity.h"
#include "Entities/PlayerEntity.h"
#include "Entities/RectEntity.h"
#include "Logging.h"
#include "Resource/EntityData.h"

namespace FA {

namespace Entity {

Factory::Factory()
{
    RegisterEntity(MoleEntity::str, [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
        return std::make_unique<MoleEntity>(id, data, std::move(s));
    });
    RegisterEntity(PlayerEntity::str,
                   [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
                       return std::make_unique<PlayerEntity>(id, data, std::move(s));
                   });
    RegisterEntity(ArrowEntity::str, [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
        return std::make_unique<ArrowEntity>(id, data, std::move(s));
    });
    RegisterEntity(CoinEntity::str, [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
        return std::make_unique<CoinEntity>(id, data, std::move(s));
    });
    RegisterEntity(RectEntity::str, [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
        return std::make_unique<RectEntity>(id, data, std::move(s));
    });
    RegisterEntity(EntranceEntity::str,
                   [](EntityId id, const Shared::EntityData& data, std::unique_ptr<EntityService> s) {
                       return std::make_unique<EntranceEntity>(id, data, std::move(s));
                   });
}

Factory::~Factory() = default;

std::unique_ptr<EntityIf> Factory::Create(const Shared::EntityData& data, std::unique_ptr<EntityService> service) const
{
    auto it = map_.find(data.typeStr_);

    if (it != map_.end()) {
        return it->second(id_++, data, std::move(service));
    }

    LOG_ERROR("Could not create entity of %s", DUMP2("type", data.typeStr_));
    return nullptr;
}

void Factory::RegisterEntity(const std::string& typeStr, Factory::CreateFn createFn)
{
    map_[typeStr] = createFn;
}

}  // namespace Entity

}  // namespace FA
