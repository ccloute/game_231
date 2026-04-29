#pragma once

#include "event.h"
#include "animatedsprite.h"

class Animation : public Event {
public:
    Animation(const Vec& position, const std::string& sprite_name);
    void on_start(Engine& engine) override;
    void execute(Engine& engine) override;

private:
    Vec position;
    std::string sprite_name;
    AnimatedSprite sprite;
};