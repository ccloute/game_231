#pragma once

#include "vec.h"
#include <vector>

class Engine;
class Entity;

class DartPathResult {
public:
    std::vector<Vec> flight_tiles;
    Entity* victim = nullptr;
};

// Tiles the dart flies through (excluding the shooter), in order, ending on a hit entity's tile if any.
// Stops before walls or closed doors (those tiles are not included).
DartPathResult compute_dart_path(Engine& engine, const Entity& attacker, Vec direction);
