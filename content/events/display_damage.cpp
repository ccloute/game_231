#include "display_damage.h"
#include "engine.h"
#include "randomness.h"

constexpr int duration = 3;

DisplayDamage::DisplayDamage(Vec position, int damage)
    : Event{duration}, position{position}, damage{damage} {
    // shift slightly up on player, randomize x to display multiple damage simultaneously
    shift = {randint(-8, 8), -12 - randint(0, 4)};
}


void DisplayDamage::execute(Engine& engine) {
    // float up
    shift -= Vec{0, 3*frame_count};
    std::string digits = std::to_string(damage);
    engine.camera.add_text(position, digits, shift);
}
