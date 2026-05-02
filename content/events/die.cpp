#include "die.h"

#include "engine.h"
#include "entity.h"

Die::Die(Entity& entity) : Event{1}, entity{entity} {}

void Die::execute(Engine& engine) {
    engine.remove_entity(entity);
}
