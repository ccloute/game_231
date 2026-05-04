#include "dart_path.h"

#include "engine.h"
#include "entity.h"
#include "tile.h"

DartPathResult compute_dart_path(Engine& engine, const Entity& attacker, Vec direction) {
    DartPathResult result;
    Vec current_position = attacker.get_position();
    while (true) {
        current_position = current_position + direction;
        if (!engine.dungeon.within_bounds(current_position)) {
            break;
        }
        Tile& tile = engine.dungeon.get_tile(current_position);
        if (tile.is_wall()) {
            break;
        }
        if (tile.has_door() && (!tile.door || !tile.door->is_open())) {
            break;
        }
        if (tile.has_entity()) {
            Entity& target = *tile.entity;
            result.flight_tiles.push_back(current_position);
            result.victim = &target;
            break;
        }
        result.flight_tiles.push_back(current_position);
    }
    return result;
}
