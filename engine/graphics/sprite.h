#pragma once

#include "vec.h"


// Represents a 2D image in the game
class Sprite {
public:
    // Spritesheet values, don't modify these!
    int texture_id{-1}; // Unique id corresponding to image file
    Vec location{0, 0}; // Upper left corner of sprite in image
    Vec size{0, 0};     // Size (width, height) of sprite in image

    // Display parameters, you can modify these!
    Vec shift{0, 0};    // Pixels to shift by when displaying
    Vec center{0, 0};   // Point of rotation
    double angle{0.0};  // Angle of rotation
    bool flip{false};   // Whether to flip sprite horizontally
};
