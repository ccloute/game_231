#include "fov.h"
#include <cmath>

int round_ties_up(double n) {
    return std::floor(n + 0.5);
}

int round_ties_down(double n) {
    return std::ceil(n - 0.5);
}

double to_decimal(const Fraction& fraction) {
    return static_cast<double>(fraction.x) / fraction.y;
}

Quadrant::Quadrant(Cardinal direction, const Vec& origin)
    :direction{direction}, origin{origin} {}

Vec Quadrant::transform(const Vec& tile) const {
    auto [row, col] = tile;
    if (direction == Cardinal::North) {
        return origin + Vec{col, -row};
    }
    else if (direction == Cardinal::South) {
        return origin + Vec{col, row};
    }
    else if (direction == Cardinal::East) {
        return origin + Vec{row, col};
    }
    else { // Cardinal::West
        return origin + Vec{-row, col};
    }
}


Row::Row(int depth, const Fraction& start_slope, const Fraction& end_slope)
    :depth{depth}, start_slope{start_slope}, end_slope{end_slope} {}

std::vector<Vec> Row::tiles() const {
    Fraction start{start_slope};
    start.x *= depth;
    int min_col = round_ties_up(to_decimal(start));
    Fraction end{end_slope};
    end.x *= depth;
    int max_col = round_ties_down(to_decimal(end));

    std::vector<Vec> tiles;
    for (int col = min_col; col <= max_col; ++col) {
        tiles.push_back({depth, col});
    }
    return tiles;
}
    
Row Row::next() const {
    return Row{depth + 1, start_slope, end_slope};
}


std::set<Vec> FieldOfView::compute(const Vec& position,
                                   std::function<bool(const Vec&)> opaque) {
    is_opaque = std::move(opaque);
    std::set<Vec> visible;
    visible.insert(position);

    for (int i = 0; i < 4; ++i) {
        Quadrant quadrant{static_cast<Quadrant::Cardinal>(i), position};
        Row first_row{1, Fraction{-1, 1}, Fraction{1, 1}};
        scan(first_row, quadrant, visible);
    }

    return visible;
}
    
bool FieldOfView::is_wall(std::optional<Vec> tile, const Quadrant& quadrant) const {
    if (!tile) {
        return false;
    }
    Vec position = quadrant.transform(tile.value());
    return is_opaque(position);
}
    
bool FieldOfView::is_floor(std::optional<Vec> tile, const Quadrant& quadrant) const {
    if (!tile) {
        return false;
    }
    Vec position = quadrant.transform(tile.value());
    return !is_opaque(position);
}


Fraction slope(const Vec& tile) {
    auto [row_depth, col] = tile;
    return Fraction{2 * col - 1, 2 * row_depth};
}
    
bool is_symmetric(const Row& row, const Vec& tile) {
    auto [row_depth, col] = tile;
    Fraction start{row.start_slope};
    start.x *= row.depth;
    Fraction end{row.end_slope};
    end.x *= row.depth;
    return col >= to_decimal(start) && col <= to_decimal(end);
}


void FieldOfView::scan(Row row, const Quadrant& quadrant, std::set<Vec>& visible) const {
    std::optional<Vec> prev_tile;
    for (Vec tile : row.tiles()) {
        if (is_wall(tile, quadrant) || is_symmetric(row, tile)) {
            Vec position = quadrant.transform(tile);
            visible.insert(position);
        }

        if (is_wall(prev_tile, quadrant) && is_floor(tile, quadrant)) {
            row.start_slope = slope(tile);
        }

        if (is_floor(prev_tile, quadrant) && is_wall(tile, quadrant)) {
            Row next_row = row.next();
            next_row.end_slope = slope(tile);
            scan(next_row, quadrant, visible);
        }

        prev_tile = tile;
    }

    if (is_floor(prev_tile, quadrant)) {
        scan(row.next(), quadrant, visible);
    }
}
