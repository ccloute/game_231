#include "wander.h"

#include "dungeon.h"
#include "engine.h"
#include "entity.h"
#include "move.h"
#include "randomness.h"
#include "rest.h"
#include "tile.h"

Result Wander::perform(Engine& engine, std::shared_ptr<Entity> entity) {
    const Vec position = entity->get_position();
    std::vector<Vec> neighbor_positions = engine.dungeon.neighbors(position);
    shuffle(std::begin(neighbor_positions), std::end(neighbor_positions));
    for (const Vec& neighbor : neighbor_positions) {
        Tile& tile = engine.dungeon.get_tile(neighbor);
        if (!tile.is_wall() && !tile.has_entity()) {
            const Vec step = neighbor - position;
            return alternative(Move{step});
        }
    }
    return alternative(Rest{});
}
