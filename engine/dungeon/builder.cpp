#include "builder.h"
#include "randomness.h"

#include <iostream>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>

[[maybe_unused]] void print_layout(const Grid<int>& layout) {
    std::cout << '+' << std::setfill('-') << std::setw(layout.width+2) << "+\n";
    for (int y = 0; y < layout.height; ++y) {
        std::cout << '|';
        for (int x = 0; x < layout.width; ++x) {
            int value = layout(x, y);
            if (value == -1) {
                std::cout << ' ';
            }
            else {
                std::cout << value;
            }
        }
        std::cout << "|\n";
    }
    std::cout << '+' << std::setfill('-') << std::setw(layout.width+2) << "+\n";
}

Builder::Builder(int room_placement_attempts)
    :room_placement_attempts{room_placement_attempts}, id{1} {}
    
std::pair<Grid<int>, Rooms> Builder::generate(int width, int height) {
    if (width % 2 == 0 || height % 2 == 0) {
        std::string msg{"screen_width and screen_height must be odd numbers: ("};
        msg += std::to_string(width) + ", " + std::to_string(height) + ")";
        throw std::runtime_error(msg);
    }

    if (width < 19 || height < 19) {
        std::string msg{"screen_width and screen_height must be at least 19: ("};
        msg += std::to_string(width) + ", " + std::to_string(height) + ")";
        throw std::runtime_error(msg);
    }

    // initialize values
    Grid<int> layout{width, height};
    rooms.clear();

    add_rooms(layout);
    create_corridors(layout);

    std::vector<Vec> connectors = reduce_connectors(find_all_connectors(layout));

    // make all walkable tiles = 1
    for (int y = 1; y < layout.height; ++y) {
        for (int x = 1; x < layout.width; ++x) {
            auto& value = layout(x, y);
            if (value != 0) {
                value = 1;
            }
        }
    }

    // make all connectors = 2
    for (const Vec& position : connectors) {
        layout(position) = 2;
    }

    remove_dead_ends(layout);

    mark_surrounded_walls(layout);

    return {layout, rooms};
}

[[maybe_unused]] std::pair<Grid<int>, Rooms> Builder::generate_test_dungeon(int width, int height) {
    Grid<int> layout{width, height};
    for (int y = 0; y < layout.height; ++y) {
        for (int x = 0; x < layout.width; ++x) {
            if (y == 0 || y == layout.height-1 || x == 0 || x == layout.width-1) {
                layout(x, y) = 0;
            }
            else {
                layout(x, y) = 1;
            }
        }
    }
    layout(3, 2) = 0; // one non-walkable tile

        
    rooms.push_back(Room{{1, 1}, {layout.width-2, layout.height-2}});

    return {layout, rooms};
}

void Builder::add_rooms(Grid<int>& layout) {
    for (int i = 0; i < room_placement_attempts; ++i) {
        Room new_room = generate_room(layout);
        if (new_room.position.x >= layout.width-2 || new_room.position.y >= layout.height-2) {
            // room is too big, doesn't fit within layout
            continue;
        }

        // check for room overlaps
        bool overlap{false};
        for (const Room& current_room : rooms) {
            if (overlaps(new_room, current_room)) {
                overlap = true;
                break;
            }
        }

        if (!overlap) {
            // room can be added to layout
            rooms.push_back(new_room);
            imprint_room(layout, new_room);
            ++id;
        }
    }
}
    
Room Builder::generate_room(const Grid<int>& layout) {
    // calculate random room dimensions
    Vec size = Vec{1, 1} * (1 + 2*randint(1, 3));

    // add some variation to the shape of rooms
    int size_variation = 2*randint(0, 1 + size.x/2);
    if (probability(50)) {
        size.x += size_variation;
    }
    else {
        size.y += size_variation;
    }

    // choose random position for room in layout
    int x = randint(0, layout.width-2 - size.x)/2 * 2 + 1;
    int y = randint(0, layout.height-2 - size.y)/2 * 2 + 1;

    Room new_room{{x, y}, size};
    return new_room;
}

void Builder::imprint_room(Grid<int>& layout, const Room& room) const {
    for (int y = 0; y < room.size.y; ++y) {
        for (int x = 0; x < room.size.x; ++x) {
            layout(x+room.position.x, y+room.position.y) = id;
        }
    }
}

void Builder::create_corridors(Grid<int>& layout) {
    auto directions = Directions;
    for (int y = 1; y < layout.height; y += 2) {
        for (int x = 1; x < layout.width; x += 2) {
            if (layout(x, y) == 0) {
                shuffle(std::begin(directions), std::end(directions));
                carve_corridor(layout, {x, y}, directions);
                ++id;
            }
        }
    }
}

void Builder::carve_corridor(Grid<int>& layout, const Vec& position,
                             std::array<Vec, 4> directions) const {
    if (layout(position) == 0) { // wall
        layout(position) = id;
            
        // choose a new random direction if out of bounds or already carved
        Vec next = position + directions[0]*2;
        if (!layout.within_bounds(next) || layout(next) != 0 || probability(10)) {
            shuffle(std::begin(directions), std::end(directions));
        }
            
        // attempt to carve corridor in each direction
        for (auto direction : directions) {
            next = position + direction*2;
            if (layout.within_bounds(next) && layout(next) == 0) {
                layout(position + direction) = id;
                carve_corridor(layout, next, directions);
            }
        }
    }
}

std::optional<Builder::Connector> Builder::maybe_connector(const Grid<int>& layout, const Vec& position) {
    if (layout(position) != 0) { // only consider walls as connectors
        return {};
    }
        
    std::unordered_set<int> neighboring_regions;
    for (const auto& direction : Directions) {
        auto neighbor = position + direction;
        int region_value = layout(neighbor);
        if (region_value > 0) {
            neighboring_regions.insert(region_value);
        }
    }
        
    if (neighboring_regions.size() == 2) {
        auto regions_iter = std::begin(neighboring_regions);
        int first = *regions_iter;
        int second = *(++regions_iter);
        return {{position, first, second}};
    }
    else {
        return {};
    }
}
 
std::vector<Builder::Connector> Builder::find_all_connectors(const Grid<int>& layout) {
    std::vector<Connector> connectors;
    for (int y = 1; y < layout.height-1; ++y) {
        for (int x = 1; x < layout.width-1; ++x) {
            Vec position{x, y};
            auto connector = maybe_connector(layout, position);
            if (connector.has_value()) {
                connectors.push_back(connector.value());
            }
        }
    }
    return connectors;
}

std::vector<Vec> Builder::reduce_connectors(const std::vector<Connector>& connectors) {
    if (connectors.empty()) {
        return {};
    }
    // Using all connectors would make too dense of dungeon, so we
    // reduce the number of connectors by merging regions.
    
    // The main data structure for this task is a graph connecting
    // region numbers to other region numbers via a list of connector
    // positions:
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<Vec>>> graph;
    //                  ^-----   regions   -----^         connected via  ^-- these connectors

    // populate graph
    for (auto [position, region_a, region_b] : connectors) {
        graph[region_a][region_b].insert(position);
        graph[region_b][region_a].insert(position);
    }

    // algorithm:
    // 1) choose a main region to which all other regions will be connected
    // 2) while the graph has more than one region
    // 3) choose other region
    // 4) choose a random connector between main and other, keep it
    // 5) with probability, choose another connector (avoids a minimum
    //    spanning tree, we want more than one path between any two
    //    points in the layout)
    //    a) if distance between this connector and previously selected one > 1, keep it
    // 6) merge main and other
    //    a) delete main from other's connected regions
    //    b) delete other from main
    //    c) add other's connectors to main
    //    d) delete other from other regions

    // connectors to keep
    std::vector<Vec> reduced_connectors;

    // choose main region
    int main = random_choice(graph).first;

    while (graph.size() > 1) {
        // choose other region
        auto [other, positions] = random_choice(graph[main]);

        // choose connector to keep
        auto position = random_choice(positions);
        reduced_connectors.push_back(position);
        graph[main][other].erase(position);
        
        // with probability, add another connector if distance > 1
        if (!positions.empty() && probability(25)) {
            auto additional = random_choice(positions);
            if (distance(position, additional) > 1) {
                reduced_connectors.push_back(additional);
            }
        }

        // merge main and other, delete other
        graph[other].erase(main);
        graph[main].erase(other);
        graph[main].merge(graph[other]);
        graph.erase(other);
        for (auto& [a, b] : graph) {
            b.erase(other);
        }
    }
    
    return reduced_connectors;
}

void Builder::remove_dead_ends(Grid<int>& layout) {
    // remove corridors that are dead ends, i.e. open tiles surrounded by 3 walls
    bool removed{true};
    while (removed) {
        removed = false;
        for (int y = 1; y < layout.height-1; ++y) {
            for (int x = 1; x < layout.width-1; ++x) {
                Vec position{x, y};
                if (layout(position) != 0) {
                    int walls{0};
                    for (auto& direction : Directions) {
                        auto neighbor = position + direction;
                        if (layout(neighbor) == 0) {
                            ++walls;
                        }
                    }
                    if (walls >= 3) {
                        layout(position) = 0;
                        removed = true;
                    }
                }
            }
        }
    }
}

void Builder::mark_surrounded_walls(Grid<int>& layout) {
    // find walls that are completely surrounded by other walls
    std::vector<Vec> surrounded;
    for (int y = 0; y < layout.height; ++y) {
        for (int x = 0; x < layout.width; ++x) {
            int sum{0};
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    if (layout.within_bounds(x+i, y+j)) {
                        sum += layout(x+i, y+j);
                    }
                }
            }
            if (sum == 0) {
                surrounded.push_back({x, y});
            }
        }
    }

    // set them to -1
    for (const auto& position : surrounded) {
        layout(position) = -1;
    }
}


