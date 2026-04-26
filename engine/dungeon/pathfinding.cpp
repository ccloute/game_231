#include "pathfinding.h"
#include "dungeon.h"

#include <queue>
#include <unordered_map>
#include <algorithm>

Path breadth_first_search(const Dungeon& dungeon, const Vec& start, const Vec& goal) {
    std::queue<Vec> frontier;
    frontier.push(start);
    std::unordered_map<Vec, Vec> came_from = {{start, start}};

    while (!frontier.empty()) {
        Vec current = frontier.front();
        frontier.pop();
        if (current == goal) {
            break;
        }

        for (const Vec& next : dungeon.neighbors(current)) {
            const Tile& tile = dungeon.tiles(next);
            if (!tile.is_wall() && came_from.count(next) == 0) {
                frontier.push(next);
                came_from[next] = current;
            }
        }
    }

    if (came_from.count(goal) == 0) {
        return {};
    }

    Vec current = goal;
    Path path;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);
    std::reverse(std::begin(path), std::end(path));
    return path;
}
