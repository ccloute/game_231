#pragma once

#include "dungeon.h"
#include "door.h"
#include "animatedsprite.h"
#include "vec.h"

#include <unordered_map>

// forward declarations
class Graphics;

// Takes the dungeon layout produced by Builder, and chooses tile
// sprites, adds decorations (e.g. torches)
class Decorator {
public:
    Decorator(const Graphics& graphics, const Grid<int>& layout,
              const std::vector<Room>& rooms);
    Dungeon create_dungeon();
        
private:
    const Graphics& graphics;
    std::vector<Room> rooms;
    Grid<Tile> tiles;
    std::unordered_map<Vec, AnimatedSprite> decorations;
        
    void set_tile_type(const Grid<int>& layout, int x, int y);
    void set_tile_sprites();
    void choose_wall_sprite(int x, int y);
    void choose_floor_sprite(int x, int y);
    void choose_door_sprite(int x, int y);
        
    void place_destroyed_wall();
    void place_torches();
    void place_pillars();
};

