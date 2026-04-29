#include "animation.h"
#include "engine.h"

Animation::Animation(const Vec& position, const std::string& sprite_name)
    :position{position}, sprite_name{sprite_name} {}

void Animation::on_start(Engine& engine) {
    sprite = engine.graphics.get_animated_sprite(sprite_name);
    number_of_frames = sprite.number_of_frames();
}

void Animation::execute(Engine& engine) {
    engine.camera.add_overlay(position, sprite.get_sprite());
    sprite.update();
}