#pragma once

#include "action.h"

class Entity;

class Attack : public Action {
public:
    explicit Attack(Entity& defender);
    Result perform(Engine& engine, std::shared_ptr<Entity> attacker) override;

private:
    Entity& defender;
};
