#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite()
    : visible{false}, sprites{Sprite{}}, ticks_per_frame{1}, current_frame{0}, time{0} {}

AnimatedSprite::AnimatedSprite(const std::vector<Sprite>& sprites, int ticks_per_frame,
                               int starting_frame)
    : visible{true},
      sprites{sprites},
      ticks_per_frame{ticks_per_frame},
      current_frame{starting_frame},
      time{0} {
    if (starting_frame >= static_cast<int>(sprites.size())) {
        std::string msg{"starting_frame (" + std::to_string(starting_frame)};
        msg += " must be less than sprites.size() " + std::to_string(sprites.size());
        throw std::runtime_error(msg);
    }
    
}


void AnimatedSprite::set_shift(Vec shift) {
    for (Sprite& sprite : sprites) {
        sprite.shift = shift;
    }
}

void AnimatedSprite::set_center(Vec center) {
    for (Sprite& sprite : sprites) {
        sprite.center = center;
    }
}

void AnimatedSprite::set_angle(double angle) {
    for (Sprite& sprite : sprites) {
        sprite.angle = angle;
    }
}

void AnimatedSprite::flip(bool flip) {
    for (Sprite& sprite : sprites) {
        sprite.flip = flip;
    }
}

void AnimatedSprite::update() {
    if (!visible) {
        return;
    }
    ++time;
    if (time >= ticks_per_frame) {
        time -= ticks_per_frame; // Keep time small
        current_frame = (current_frame + 1) % static_cast<int>(sprites.size());
    }
}

Sprite AnimatedSprite::get_sprite() const {
    return sprites.at(current_frame);
}

int AnimatedSprite::number_of_frames() const {
    return static_cast<int>(sprites.size());
}
