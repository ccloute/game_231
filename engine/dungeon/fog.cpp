#include "fog.h"
#include "dungeon.h"
#include "fov.h"
#include <algorithm>

Fog::Fog(double brightness_seen)
    :brightness_seen{brightness_seen}, position{} {}

void Fog::update_visibility(Dungeon& dungeon, const Vec& new_position) {
    position = new_position;
    for (const Vec& pos : visible_tiles) {
        if (dungeon.within_bounds(pos)) {
            dungeon.tiles(pos).visible = false;
        }            
    }

    previously_seen_tiles.insert(std::begin(visible_tiles), std::end(visible_tiles));

    FieldOfView fov;
    std::set<Vec> fov_results = fov.compute(new_position, [&](const Vec& pos) {
        return dungeon.within_bounds(pos) && dungeon.is_opaque(pos);
    });
    for (const Vec& pos : fov_results) {
        if (dungeon.within_bounds(pos)) {
            visible_tiles.insert(pos);
            dungeon.tiles(pos).visible = true;
        }            
    }
}

double Fog::brightness(const Vec& location) const {
    if (visible_tiles.count(location)) {
        double dist = distance(position, location);
        double value = std::clamp(0.1*(dist-1), 0.0, brightness_seen);
        return value;
    }
    else if (previously_seen_tiles.count(location)) {
        return brightness_seen;
    }
    else {
        return 1;
    }
}
