#pragma once

#include "event.h"

class Engine;
class Entity;

void apply_dart_poison(Engine& engine, Entity& victim);

// Damage-over-time from poison dart: 2 HP on each of the victim's next two completed turns
// (immediate 2 HP is applied by apply_dart_poison before this condition is added/refreshed).
class PoisonDebuff : public Event {
public:
    PoisonDebuff(Entity& entity, int ticks_remaining);
    void execute(Engine& engine) override;
    void when_done(Engine& engine) override;
    bool is_done() const override;
    void refresh_ticks(int ticks);

private:
    Entity& entity;
    int ticks_remaining;
};
