#include "closedoor.h"

#include "dungeon.h"
#include "engine.h"
#include "entity.h"
#include "tile.h"
#include "updatefov.h"

Result CloseDoor::perform(Engine& engine, std::shared_ptr<Entity> entity) {
    bool closed_any_doors = false;
    const Vec pos = entity->get_position();
    for (const Vec& neighbor : engine.dungeon.neighbors(pos)) {
        Tile& tile = engine.dungeon.get_tile(neighbor);
        if (tile.has_door() && tile.door && tile.door->is_open()) {
            tile.door->close();
            closed_any_doors = true;
        }
    }
    if (closed_any_doors) {
        engine.events.create_event<UpdateFOV>();
        return success();
    }
    return failure();
}
