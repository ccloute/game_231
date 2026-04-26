#pragma once

#include "sprite.h"
#include <vector>

// A looping animation based on a series of sprites
class AnimatedSprite {
public:
    AnimatedSprite();

    // ticks_per_frame is how often to move to the next sprite when update() is called.
    // = 1 means one call to update() moves to the next frame
    // = 2 means two calls to update() moves to the next frame
    AnimatedSprite(const std::vector<Sprite>& sprites, int ticks_per_frame=1, int starting_frame=0);

    // transformations on each sprite in sequence
    void set_shift(Vec shift);
    void set_center(Vec center);
    void set_angle(double angle);
    void flip(bool flip);

    // move to next frame of animation
    void update();
    
    [[nodiscard]] Sprite get_sprite() const;
    [[nodiscard]] int number_of_frames() const;

    // Set whether draw
    bool visible;
        
private:
    std::vector<Sprite> sprites;
    int ticks_per_frame, current_frame, time;
};
