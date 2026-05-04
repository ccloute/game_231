#pragma once

#include "item.h"

class PoisonDart : public Item {
public:
    PoisonDart();
    void use(Engine& engine, Entity& owner) override;
};
