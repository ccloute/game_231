#include "decorator.h"
#include "graphics.h"
#include "randomness.h"

#include <unordered_map>


Decorator::Decorator(const Graphics& graphics, const Grid<int>& layout,
                     const std::vector<Room>& rooms)
    :graphics{graphics}, rooms{rooms}, tiles{layout.width, layout.height} {

    for (int y = 0; y < layout.height; ++y) {
        for (int x = 0; x < layout.width; ++x) {
            set_tile_type(layout, x, y);
        }
    }
}

Dungeon Decorator::create_dungeon() {
    set_tile_sprites();

    place_torches();
    place_pillars();
        
    // randomly place some destroyed walls
    for (int n = 0; n < 2; ++n) {
        if (probability(50)) {
            place_destroyed_wall();
        }
    }

    return Dungeon{tiles, rooms, decorations};
}

void Decorator::set_tile_type(const Grid<int>& layout, int x, int y) {
    Tile& tile = tiles(x, y);
    int value = layout(x, y);
    if (value == -1) {
        tile.type = Tile::Type::None;
    }
    else if (value == 0) {
        tile.type = Tile::Type::Wall;
    }
    else if (value == 1) {
        tile.type = Tile::Type::Floor;
    }
    else if (value == 2) {
        tile.type = Tile::Type::Door;
    }
}

void Decorator::set_tile_sprites() {
    for (int y = 0; y < tiles.height; ++y) {
        for (int x = 0; x < tiles.width; ++x) {
            auto& tile = tiles(x, y);
            if (tile.type == Tile::Type::Wall) {
                choose_wall_sprite(x, y);
            }
            else if (tile.type == Tile::Type::Floor) {
                choose_floor_sprite(x, y);
            }
            else if (tile.type == Tile::Type::Door) {
                choose_door_sprite(x, y);
            }
        }
    }
}

    
    
void Decorator::choose_wall_sprite(int x, int y) {
    // build up name of tile by counting the surrounding walls
    const std::array<std::string, 4> direction_names = {
        "_right", "_up", "_left", "_down"
    };
    std::string name;
    for (std::size_t i = 0; i < direction_names.size(); ++i) {
        auto neighbor = Vec{x, y} + Directions.at(i);
        if (tiles.within_bounds(neighbor) && tiles(neighbor).is_wall()) {
            name += direction_names.at(i);
        }
    }

    // handle special cases
    if (name.empty()) { // no surrounding walls, therefore it's a pillar
        name = "_pillar";
    }
    else if (name == "_right_left" || name == "_up_down") {
        // randomly add fancy cracked walls
        int prob = randint(0, 99);
        if (prob < 10) {
            name += "_3";
        }
        else if (prob < 20) {
            name += "_2";
        }
        else {
            name += "_1";
        }
    }
        
    tiles(x, y).sprite = graphics.get_sprite("wall" + name);
}
    

void Decorator::choose_floor_sprite(int x, int y) {
    int probability = randint(0, 99);
    std::string tile_name;
    if (probability < 5) {
        tile_name = "floor_cracked_1";
    }
    else if (probability < 20) {
        tile_name = "floor_sunken";
    }
    else if (probability < 35) {
        tile_name = "floor_cracked_2";
    }
    else {
        tile_name = "floor_nice";
    }
    tiles(x, y).sprite = graphics.get_sprite(tile_name);
}
    
void Decorator::choose_door_sprite(int x, int y) {
    choose_floor_sprite(x, y);
    bool is_horizontal{false};
    if (tiles(x-1, y).is_wall() && tiles(x+1, y).is_wall()) {
        is_horizontal = true;
    }
    Tile& tile = tiles(x, y);
    tile.door = std::make_shared<Door>(tile, is_horizontal,  graphics.get_sprite("door_horizontal"),
                                       graphics.get_sprite("door_vertical"));
}
    
void Decorator::place_destroyed_wall() {
    // find positions where horizontal walls separate two regions of walkable tiles
    std::vector<Vec> positions;
    for (int y = 1; y < tiles.height-1; ++y) {
        int walls{0};
        for (int x = 0; x < tiles.width; ++x) {
            if (tiles(x, y).is_wall()) {
                ++walls;
            }
            else {
                walls = 0;
            }
                
            if (walls == 5) {
                // check if above and below are floor tiles
                bool good{true};
                for (int dx = -3; dx <= -1; ++dx) {
                    good &= tiles(x+dx, y+1).is_walkable() && tiles(x+dx, y-1).is_walkable();
                }
                if (good) {
                    positions.push_back({x-2, y});
                }
            }
        }
    }
        
    if (positions.empty()) {
        return;
    }
        
    auto position = random_choice(positions);
    auto [x, y] = position;
    const std::vector<std::string> x_names = {"_left", "_center", "_right"};
    const std::vector<std::string> y_names = {"_bottom", "_center", "_top"};
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            std::string name{"floor_broken" + y_names.at(dy+1) + x_names.at(dx+1)};
            tiles(x+dx, y+dy).sprite = graphics.get_sprite(name);
        }
    }
    tiles(x, y).type = Tile::Type::Floor;

    // remove any surrounding decorations
    decorations.erase({x-1, y});
    decorations.erase({x+1, y});

    decorations[{x-1, y}] = graphics.get_animated_sprite({"wall_destroyed_left"}, 1);
    decorations[{x, y}] = graphics.get_animated_sprite({"wall_destroyed_center"}, 1);
    decorations[{x+1, y}] = graphics.get_animated_sprite({"wall_destroyed_right"}, 1);
}

void Decorator::place_torches() {
    // find positions where 3 horizontal walls with floors below
    std::vector<Vec> positions;
    for (int y = 1; y < tiles.height; ++y) {
        int walls = 0;
        for (int x = 1; x < tiles.width-1; ++x) {
            if (tiles(x, y).is_wall() && tiles(x, y-1).is_walkable()) {
                ++walls;
            }
            else {
                walls = 0;
            }
            if (walls == 3) {
                positions.push_back({x-1, y});
                walls = 0;
            }
        }
    }

    std::string sprite_name{"torch"};
    int ticks_per_frame{2};
    bool randomize{true};
    for (auto& position : positions) {
        if (probability(50)) {
            decorations[position] = graphics.get_animated_sprite(sprite_name, ticks_per_frame, randomize, randomize);
            
        }
    }
}


void Decorator::place_pillars() {
    for (auto& room : rooms) {
        // no pillars in small rooms
        // min = 5x7 or 7x5
        int min = std::min(room.size.x, room.size.y);
        int max = std::max(room.size.x, room.size.y);
        if (min < 5 || max < 7) {
            continue;
        }

        // place pillars every 4 spaces for rooms: size -> index
        // 5 -> (2)
        // 7 -> (1, 5)
        // 9 -> (2, 6)
        // 11 -> (1, 5, 9)
        // 13 -> (2, 6, 10)
        // 15 -> (1, 5, 9, 13)
        // if size/2 is even, start at 2 instead of 1
        int start_x = 1;
        if ((room.size.x / 2) % 2 == 0) {
            start_x = 2;
        }
        int start_y = 1;
        if ((room.size.y / 2) % 2 == 0) {
            start_y = 2;
        }

        int spacing = 4;
        for (int y = start_y; y < room.size.y; y += spacing) {
            for (int x = start_x; x < room.size.x-1; x += spacing) {
                Vec position = room.position + Vec{x, y};
                auto& tile = tiles(position);
                tile.type = Tile::Type::Wall;
                tile.sprite = graphics.get_sprite("wall_pillar");
            }
        }
    }
}
