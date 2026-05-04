#include "poison_dart.h"

#include "dart_path.h"
#include "engine.h"
#include "entity.h"
#include "poison_dart_projectile.h"

PoisonDart::PoisonDart() : Item{"bow"} {}

void PoisonDart::use(Engine& engine, Entity& owner) {
    const Vec direction = owner.get_direction();
    DartPathResult path = compute_dart_path(engine, owner, direction);
    engine.events.create_event<PoisonDartProjectile>(path.flight_tiles, path.victim, direction);
}
