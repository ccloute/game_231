#pragma once

#include "event.h"
#include "vec.h"

class DisplayDamage : public Event {
public:
    DisplayDamage(Vec position, int damage);
    void execute(Engine& engine) override;

private:
    Vec position, shift;
    int damage;
};
