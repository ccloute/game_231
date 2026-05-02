#include "hit.h"

#include "die.h"
#include "entity.h"

Hit::Hit(Entity& entity, int damage)
    : Event{1}, entity{entity}, damage{damage} {}

void Hit::execute(Engine&) {
    entity.take_damage(damage);
}

void Hit::when_done(Engine&) {
    if (!entity.is_alive()) {
        add_next<Die>(entity);
    }
}
