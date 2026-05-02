#include "sword.h"

#include "engine.h"
#include "entity.h"
#include "hit.h"
#include "thrust.h"

Sword::Sword(int damage) : Item{"sword"}, damage{damage} {}

void Sword::use(Engine& engine, Entity& attacker, Entity& defender) {
    const Vec direction = defender.get_position() - attacker.get_position();
    auto thrust = engine.events.create_event<Thrust>(sprite, direction);
    thrust->add_next<Hit>(defender, damage);
}
