#include "graphics.h"
#include "vec.h"
#include "randomness.h"

#include <SDL3/SDL.h>

#include <sstream>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <filesystem>

Graphics::Graphics(const std::string& title, int screen_width, int screen_height)
    : screen_width{screen_width}, screen_height{screen_height} {
    window = SDL_CreateWindow(title.c_str(), screen_width, screen_height, 0);
    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

Graphics::~Graphics() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}


void Graphics::load_sprite_sheet(const std::string& filename) {
    std::filesystem::path file_path{filename};
    std::ifstream input{file_path};
    if (!input) {
        throw std::runtime_error("Could not open filename: " + filename);
    }

    // first line is the png filename (relative path)
    std::string image_filename;
    input >> image_filename;
    std::filesystem::path image_path = file_path.parent_path() / image_filename;

    int texture_id = get_texture_id(image_path);
    
    // load sprites
    for (std::string line; std::getline(input, line);) {
        if (line.empty() || std::all_of(std::begin(line), std::end(line), isspace)) {
            continue;
        }
        std::istringstream input{line};
        std::string name;
        int x, y, width, height;
        input >> name >> x >> y >> width >> height;
        Vec shift{-width / 2, -height}; // anchor at bottom center of sprite
        Vec center{width / 2, height / 2};
            
        int number_of_frames;
        if (!(input >> number_of_frames)) { // attempt to read optional value
            number_of_frames = 1;
            input.clear(); // clear error if there was no optional value
        }
        for (int i = 0; i < number_of_frames; ++i) {
            Vec location{x+i*width, y};
            Vec size{width, height};
            Sprite sprite{texture_id, location, size, shift, center};
            sprites[name].push_back(sprite);
        }
    }

    if (sprites.empty()) {
        throw std::runtime_error("Could not read any sprites from filename: " + filename);
    }
}

Sprite Graphics::get_sprite(const std::string& name) const {
    auto i = sprites.find(name);
    if (i == sprites.end()) {
        throw std::runtime_error("Cannot find sprite: " + name);
    }
    return i->second.front();
}

AnimatedSprite Graphics::get_animated_sprite(const std::string& name, int ticks_per_frame,
                                             bool random_starting_frame, bool shuffle_order) const {
    auto i = sprites.find(name);
    if (i == sprites.end()) {
        throw std::runtime_error("Cannot find sprite: " + name);
    }

    std::vector<Sprite> sprite_series = i->second;
    if (shuffle_order) {
        shuffle(std::begin(sprite_series), std::end(sprite_series));
    }
        
    if (sprite_series.size() > 1 && random_starting_frame) {
        // choose random starting frame
        int starting_frame = randint(0, static_cast<int>(sprite_series.size()) - 1);
        return AnimatedSprite{sprite_series, ticks_per_frame, starting_frame};
    }
    else {
        return AnimatedSprite{sprite_series, ticks_per_frame};
    }
}

Sprite Graphics::load_image(const std::string& filename) {
    int id = get_texture_id(filename);
    SDL_Texture* texture = textures.at(id);
    Sprite sprite{id, {0, 0}, {texture->w, texture->h}};
    return sprite;
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Graphics::redraw() {
    SDL_RenderPresent(renderer);
}

void Graphics::draw_rect(const Vec& pixel, const Vec& size, int r, int g, int b, int alpha) {
    const SDL_FRect rect{static_cast<float>(pixel.x), static_cast<float>(pixel.y),
                         static_cast<float>(size.x), static_cast<float>(size.y)};
    SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
    SDL_RenderFillRect(renderer, &rect);
}


void Graphics::draw_sprite(const Vec& pixel, const Sprite& sprite, int scale) {
    if (sprite.texture_id < 0) {  // sprite with empty texture
        return;
    }
    int x = pixel.x + sprite.shift.x * scale;
    int y = pixel.y + sprite.shift.y * scale;
    int w = sprite.size.x * scale;
    int h = sprite.size.y * scale;
    SDL_FRect screen_pixels{static_cast<float>(x), static_cast<float>(y), static_cast<float>(w),
                            static_cast<float>(h)};
    SDL_FPoint center{static_cast<float>(sprite.center.x * scale),
                      static_cast<float>(sprite.center.y * scale)};
    SDL_FRect image_pixels{static_cast<float>(sprite.location.x),
                           static_cast<float>(sprite.location.y), static_cast<float>(sprite.size.x),
                           static_cast<float>(sprite.size.y)};
    SDL_Texture* texture = textures.at(sprite.texture_id);
    SDL_FlipMode flip = sprite.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderTextureRotated(renderer, texture, &image_pixels, &screen_pixels, sprite.angle,
                             &center, flip);
}

void Graphics::draw_image(const Sprite& sprite) {
    SDL_Texture* texture = textures.at(sprite.texture_id);
    SDL_FRect image_pixels{0, 0, static_cast<float>(sprite.size.x), static_cast<float>(sprite.size.y)};
    SDL_FRect screen_pixels{0, 0, static_cast<float>(screen_width), static_cast<float>(screen_height)};
    SDL_RenderTexture(renderer, texture, &image_pixels, &screen_pixels);
}

int Graphics::get_texture_id(const std::filesystem::path& image_filename) {
    auto search = texture_ids.find(image_filename.string());
    if (search != texture_ids.end()) {  // image file already loaded
        int texture_id = search->second;
        return texture_id;
    } else {  // new image file
        // SDL_Texture* texture = IMG_LoadTexture(renderer, image_filename.c_str());
        SDL_Surface* surface = SDL_LoadPNG(image_filename.string().c_str());
        if (!surface) {
            std::cerr << "Warning: Unable to load image " << image_filename
                      << " (" << SDL_GetError() << "). Using placeholder texture.\n";
            surface = SDL_CreateSurface(16, 16, SDL_PIXELFORMAT_RGBA32);
            if (!surface) {
                throw std::runtime_error(SDL_GetError());
            }
            SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 255, 0, 255, 255));
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        if (!texture) {
            throw std::runtime_error(SDL_GetError());
        }
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        std::cout << "New: " << image_filename << ": " << texture << "\n";

        // register new texture
        int texture_id = static_cast<int>(textures.size());
        texture_ids[image_filename.string()] = texture_id;
        // retain ownership of texture pointer
        textures.push_back(texture);
        return texture_id;
    }
}
