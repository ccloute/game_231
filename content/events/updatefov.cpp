#include "updatefov.h"

#include "engine.h"
#include "entity.h"

void UpdateFOV::execute(Engine& engine) {
    engine.dungeon.update_visibility(engine.hero->get_position());
}
