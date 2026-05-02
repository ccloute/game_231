#pragma once

#include "action.h"

class Wander : public Action {
public:
    Result perform(Engine& engine, std::shared_ptr<Entity> entity) override;
};
