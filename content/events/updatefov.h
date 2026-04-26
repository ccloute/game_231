#pragma once

#include "event.h"

class UpdateFOV : public Event {
public:
    void execute(Engine& engine) override;
};
