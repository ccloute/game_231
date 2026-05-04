#pragma once

#include "event.h"
#include "sprite.h"
#include "vec.h"
#include <vector>

class Entity;
class Engine;

// Draws the arrow along `path` for a few frames, then applies poison dart damage if `victim`
// is still alive.
class PoisonDartProjectile : public Event {
public:
    PoisonDartProjectile(std::vector<Vec> path_in, Entity* victim_in, Vec direction_in);
    void on_start(Engine& engine) override;
    void execute(Engine& engine) override;
    void when_done(Engine& engine) override;

private:
    static constexpr int frames_per_tile = 2;
    std::vector<Vec> path;
    Entity* victim;
    Vec direction;
    Sprite arrow;
};
