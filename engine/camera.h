#pragma once

#include "vec.h"
#include "sprite.h"
#include <vector>
#include <memory>

// forward declaration
class Graphics;
class Dungeon;
class Entities;
class Engine;
class Item;


class Camera {
public:
    Camera(Graphics& graphics, int tile_size, int zoom=1);

    // drawing functions
    void render(const Vec& position, const Sprite& sprite) const;
    void render(const Dungeon& dungeon) const;
    void render(const Entities& entities) const;
    void render_fog(const Dungeon& dungeon) const;
    void render_rect(const Vec& position, int red, int green, int blue, int alpha) const;
    void render_health_bar(int current_health, int max_health);
    void render_items(int selected_item, const std::vector<std::shared_ptr<Item>>& items);
    
    void add_overlay(const Vec& position, const Sprite& sprite);
    void render_overlays();

    void add_text(const Vec& position, const std::string& text, const Vec& shift={0, 0});
    void render_texts();
    

    void update();

    // translate entity positions to pixel positions
    [[nodiscard]] Vec world_to_screen(const Vec& position) const;

    // position camera over this position
    void move_to(const Vec& position);
    void zoom_in();
    void zoom_out();

    
private:
    Graphics& graphics;
    int tile_size;
    int zoom;
    Vec location, screen_center;

    Vec min, max; // world coordinates of min/max tiles that are visible
    void calculate_visibility_limits();
    [[nodiscard]] bool within_view(const Vec& position) const;

    std::vector<std::pair<Vec, Sprite>> overlays;
    std::vector<std::pair<Vec, Sprite>> texts;
};

