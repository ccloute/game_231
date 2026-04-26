#pragma once

#include "vec.h"
#include <set>

class Dungeon;

// Manages tiles visibility and brightness
class Fog {
public:
    explicit Fog(double brightness_seen=0.7);
    void update_visibility(Dungeon& dungeon, const Vec& position);

    [[nodiscard]] double brightness(const Vec& location) const;
    
private:
    double brightness_seen;
    Vec position;
    std::set<Vec> visible_tiles, previously_seen_tiles;
};
