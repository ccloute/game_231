#include "move.h"

#include "attack.h"
#include "engine.h"
#include "entity.h"
#include "opendoor.h"
#include "rest.h"
#include "tile.h"

Move::Move(Vec direction) : direction{direction} {}

Result Move::perform(Engine& engine, std::shared_ptr<Entity> entity) {
    entity->change_direction(direction);
    const Vec target = entity->get_position() + direction;
    Tile& tile = engine.dungeon.get_tile(target);
    if (tile.has_door() && tile.door && !tile.door->is_open()) {
        return alternative(OpenDoor{*tile.door});
    }
    if (tile.has_entity()) {
        Entity& occupant = *tile.entity;
        if (occupant.get_team() != entity->get_team()) {
            return alternative(Attack{occupant});
        }
        return alternative(Rest{});
    }
    if (tile.is_wall() || !tile.is_walkable()) {
        return failure();
    }
    entity->move_to(target);
    return success();
}
