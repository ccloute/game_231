#include "mace.h"

#include "engine.h"
#include "entity.h"
#include "hit.h"
#include "thrust.h"

Mace::Mace(int damage) : Item{"mace"}, damage{damage} {}

void Mace::use(Engine& engine, Entity& attacker, Entity& defender) {
    const Vec direction = defender.get_position() - attacker.get_position();
    auto thrust = engine.events.create_event<Thrust>(sprite, direction);
    thrust->add_next<Hit>(defender, damage);
}
