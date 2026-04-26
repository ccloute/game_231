#pragma once

#include "vec.h"
#include "room.h"
#include "tile.h"
#include "grid.h"
#include "door.h"
#include "animatedsprite.h"
#include "fog.h"
#include "pathfinding.h"
#include <vector>
#include <unordered_map>

class Dungeon {
public:
    Dungeon() = default;
    Dungeon(const Grid<Tile>& tiles, const std::vector<Room>& rooms,
            const std::unordered_map<Vec, AnimatedSprite>& decorations);

    // Returns the position of an unoccupied tile
    Vec random_open_room_tile() const;

    // Update tile sprites or conditions
    void update();

    // Recompute fog of war
    void update_visibility(const Vec& position);

    // Removes an entity from the given tile position
    void remove_entity(const Vec& position);

    // Access to dungeon tiles
    Tile& get_tile(const Vec& position);
    
    // Return all neighboring tile positions that are within bounds
    bool within_bounds(const Vec& position) const;
    std::vector<Vec> neighbors(const Vec& position) const;

    // Returns if tile at position blocks visibility
    bool is_opaque(const Vec& position) const;

    // Computes a series of open tiles from start to stop
    Path calculate_path(const Vec& start, const Vec& stop) const;

    // Individual tiles can be access using integers x and y: Tile& tile = tiles(x, y)
    // OR
    // Vec position{x, y}; Tile& tile = tiles(position)
    Grid<Tile> tiles;

    // Other elements of the dungeon
    std::vector<Room> rooms;
    std::unordered_map<Vec, AnimatedSprite> decorations; // decorations on tiles (e.g. torches)
    Fog fog; // used for computing which tiles are visible for hero
};
