
#include "item.h"
#include "tile.h"
Spear::Spear(int damage)
    : Item{"spear"}, damage{damage}{}

void Spear::use(Engine& engine, Engine& attacker, Entity& defender) {
    Vec direction = defender.get_position() - attacker.get_position();
    auto thrust = engine.events.create_event<Swing>(sprite, direction);
    thrust->add_nex<Hit>(defender, damage);
}

void Spear::use(Engine& engine, Entity& owner) {
    Vec position = owner.get_position() + Vec{1,0};
    Tile& tile = engine.dungeon.get_tile(position);

    if (tile.is_wall()) {
        return;
    }
    engine.events.create_event<AudioEvent>
    auto fire = engine.events.create_event<Animation>(position, "fire");
    if (tile.has_event()) {
        fire->add_next<Hit>(*tile.entity, damage);
    }
}