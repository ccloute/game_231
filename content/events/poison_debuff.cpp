#include "poison_debuff.h"

#include "engine.h"
#include "entity.h"
#include "hit.h"

#include <unordered_map>

static std::unordered_map<const Entity*, std::weak_ptr<PoisonDebuff>> active_poison;

static void unregister_poison(const Entity* victim) {
    active_poison.erase(victim);
}

void apply_dart_poison(Engine& engine, Entity& victim) {
    victim.take_damage(2);
    if (!victim.is_alive()) {
        unregister_poison(&victim);
        engine.remove_entity(victim);
        return;
    }

    const auto existing_poison_it = active_poison.find(&victim);
    if (existing_poison_it != active_poison.end()) {
        if (const auto active_debuff = existing_poison_it->second.lock()) {
            active_debuff->refresh_ticks(2);
            return;
        }
        active_poison.erase(existing_poison_it);
    }

    const std::shared_ptr<PoisonDebuff> new_debuff = victim.add_condition<PoisonDebuff>(victim, 2);
    active_poison[&victim] = new_debuff;
}

PoisonDebuff::PoisonDebuff(Entity& entity, int ticks_remaining)
    : Event{999999}, entity{entity}, ticks_remaining{ticks_remaining} {}

void PoisonDebuff::refresh_ticks(const int ticks) {
    ticks_remaining = ticks;
}

void PoisonDebuff::execute(Engine& engine) {
    if (ticks_remaining <= 0 || !entity.is_alive()) {
        return;
    }
    engine.events.create_event<Hit>(entity, 2);
    --ticks_remaining;
}

void PoisonDebuff::when_done(Engine&) {
    unregister_poison(&entity);
}

bool PoisonDebuff::is_done() const {
    return !entity.is_alive() || ticks_remaining <= 0;
}
