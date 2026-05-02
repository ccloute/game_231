#pragma once

#include "item.h"

class Engine;
class Entity;

class Mace : public Item {
public:
    Mace(int damage);

    void use(Engine& engine, Entity& attacker, Entity& defender) override;

private:
    int damage;
};
