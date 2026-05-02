#pragma once

#include "item.h"

class Engine;
class Entity;

class Sword : public Item {
public:
    Sword(int damage);

    void use(Engine& engine, Entity& attacker, Entity& defender) override;

private:
    int damage;
};
