#pragma once

#include "event.h"

class Entity;

class Die : public Event {
public:
    explicit Die(Entity& entity);
    void execute(Engine& engine) override;

private:
    Entity& entity;
};
