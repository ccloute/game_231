#include "thrust.h"

namespace {
constexpr int duration = 3;
constexpr int delta = 3;
}

Thrust::Thrust(Sprite& sprite, Vec direction)
    : Event{duration}, sprite{sprite}, copy{sprite}, direction{direction} {}

void Thrust::execute(Engine&) {
    sprite.shift += direction * delta;
}

void Thrust::when_done(Engine&) {
    sprite = copy;
}
