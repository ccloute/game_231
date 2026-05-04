#include "poison_dart_projectile.h"

#include "engine.h"
#include "entity.h"
#include "poison_debuff.h"
#include "vec.h"

PoisonDartProjectile::PoisonDartProjectile(std::vector<Vec> path_in, Entity* victim_in, Vec direction_in)
    : Event{path_in.empty() ? 1 : static_cast<int>(path_in.size()) * frames_per_tile},
      path{path_in},
      victim{victim_in},
      direction{direction_in} {}

void PoisonDartProjectile::on_start(Engine& engine) {
    arrow = engine.graphics.get_sprite("arrow");
    // Arrow points north by default
    arrow.angle = angle(direction);
    arrow.flip = false;
}

void PoisonDartProjectile::execute(Engine& engine) {
    if (path.empty()) {
        return;
    }
    const int last = static_cast<int>(path.size()) - 1;
    int index = frame_count / frames_per_tile;
    if (index > last) {
        index = last;
    }
    engine.camera.add_overlay(path[static_cast<std::size_t>(index)], arrow);
}

void PoisonDartProjectile::when_done(Engine& engine) {
    if (victim != nullptr && victim->is_alive()) {
        apply_dart_poison(engine, *victim);
    }
}
