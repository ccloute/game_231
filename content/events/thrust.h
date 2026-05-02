#pragma once

#include "event.h"
#include "sprite.h"
#include "vec.h"

class Thrust : public Event {
public:
    Thrust(Sprite& sprite, Vec direction);
    void execute(Engine& engine) override;
    void when_done(Engine& engine) override;

private:
    Sprite& sprite;
    Sprite copy;
    Vec direction;
};
