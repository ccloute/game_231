#pragma once

#include <optional>
#include <set>
#include <vector>
#include <functional>
#include "vec.h"

// forward declarations
class Dungeon;

// Albert Ford's Symmetric Shadow-casting
class Quadrant {
public:
    enum class Cardinal { North=0, East, South, West };

    Quadrant(Cardinal direction, const Vec& origin);

    // converts tile coords (row, col) to dungeon coords (x, y)
    // for the given Cardinal direction
    [[nodiscard]] Vec transform(const Vec& tile) const;

    Cardinal direction;
    Vec origin;
};

using Fraction = Vec;

class Row {
public:
    Row(int depth, const Fraction& start_slope, const Fraction& end_slope);
    [[nodiscard]] std::vector<Vec> tiles() const;
    [[nodiscard]] Row next() const;

    int depth;
    Fraction start_slope, end_slope;
};

class FieldOfView {
public:
    [[nodiscard]] std::set<Vec> compute(const Vec& position,
                                        std::function<bool(const Vec&)> is_opaque);

private:
    std::function<bool(const Vec&)> is_opaque;
    [[nodiscard]] bool is_wall(std::optional<Vec> tile, const Quadrant& quadrant) const;
    [[nodiscard]] bool is_floor(std::optional<Vec> tile, const Quadrant& quadrant) const;
    void scan(Row row, const Quadrant& quadrant, std::set<Vec>& visible) const;
};
