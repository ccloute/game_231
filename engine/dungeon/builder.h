#pragma once

#include "grid.h"
#include "vec.h"
#include "room.h"

#include <vector>
#include <tuple>
#include <optional>

[[maybe_unused]] void print_layout(const Grid<int>& layout);

// Class to create the structure of the dungeon (rooms and corridors)
class Builder {
public:
    explicit Builder(int room_placement_attempts);
    std::pair<Grid<int>, Rooms> generate(int width, int height);
    [[maybe_unused]] std::pair<Grid<int>, Rooms> generate_test_dungeon(int width, int height);

private:
    int room_placement_attempts;
    int id;
    Rooms rooms;

    void add_rooms(Grid<int>& layout);
    [[nodiscard]] static Room generate_room(const Grid<int>& layout) ;
    void imprint_room(Grid<int>& layout, const Room& room) const;

    void create_corridors(Grid<int>& layout);
    void carve_corridor(Grid<int>& layout, const Vec& position,
                        std::array<Vec, 4> directions) const;

    using Connector = std::tuple<Vec, int, int>;
    [[nodiscard]] static std::optional<Connector> maybe_connector(const Grid<int>& layout, const Vec& position);
    [[nodiscard]] static std::vector<Connector> find_all_connectors(const Grid<int>& layout) ;
    [[nodiscard]] static std::vector<Vec> reduce_connectors(const std::vector<Connector>& connectors) ;
        
    static void remove_dead_ends(Grid<int>& layout) ;
    static void mark_surrounded_walls(Grid<int>& layout) ;
};

