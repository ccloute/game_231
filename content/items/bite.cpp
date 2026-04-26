#include "bite.h"

Bite::Bite(int damage)
    :Item{"none"}, damage{damage} {}

void Bite::use(Engine&, Entity&, Entity&) {
}
