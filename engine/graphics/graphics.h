#pragma once

#include "sprite.h"
#include "animatedsprite.h"
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

// forward declarations
class Vec;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

// Class for loading images and drawing to the screen. Note this class
// only knows about pixel coordinates. Use the Camera class to draw
// items at specific dungeon coordinates.
class Graphics {
public:
    // window bar title and pixel screen_width and screen_height
    Graphics(const std::string& title, int screen_width, int screen_height);
    ~Graphics();

    // Image and spritesheet management
    void load_sprite_sheet(const std::string& filename);
    Sprite get_sprite(const std::string& name) const;

    // ticks_per_frame is inversely proportional to the animation
    // rate, i.e. 2 is twice as slow as 1.
    AnimatedSprite get_animated_sprite(const std::string& name, int ticks_per_frame=1,
                                       bool random_starting_frame=false,
                                       bool shuffle_order=false) const;
    Sprite load_image(const std::string& filename);

    // Drawing primitives
    void clear();
    void draw_rect(const Vec& pixel, const Vec& size, int r, int g, int b, int alpha);
    void draw_sprite(const Vec& pixel, const Sprite& sprite, int scale=1);
    void draw_image(const Sprite& sprite); // draws fullscreen
    void redraw(); // redraw all sprites

    const int screen_width;
    const int screen_height;
        
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures; // image files are stored as Textures

    // Image filename and index into textures vector
    std::unordered_map<std::string, int> texture_ids;

    // Each sprite has a name and corresponds to a series of Sprites
    // (length >= 1). This allows for AnimatedSprite and Sprite to be
    // treated equally.
    std::unordered_map<std::string, std::vector<Sprite>> sprites;

    int get_texture_id(const std::filesystem::path& image_filename);
};

