#include "potion.h"

#include "entity.h"

Potion::Potion(int healing_amount) : Item{"potion_red"}, healing_amount{healing_amount} {}

void Potion::use(Engine&, Entity& owner) {
    owner.take_damage(-healing_amount);
}
