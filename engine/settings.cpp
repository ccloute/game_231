#include "settings.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

Settings::Settings(const std::filesystem::path& filename)
    :path{std::filesystem::absolute(filename)} {
    load();
}

void Settings::load() {
    std::ifstream input{path};
    if (!input) {
        std::string msg{"Could not open settings file: "};
        msg += (std::filesystem::current_path() / path).string();
        throw std::runtime_error(msg);
    }

    for (std::string line; std::getline(input, line);) {
        std::istringstream iss{line};
        std::string key, value;
        if (iss >> key >> value) {
            parameters[key] = value;
        }
        else {
            throw std::runtime_error("Invalid line in settings.txt: " + line);
        }
    }

    load("title", title);
    load("screen_width", screen_width);
    load("screen_height", screen_height);
    load("tile_size", tile_size);
    load("zoom", zoom);
    load("tiles", tiles);
    load("heroes", heroes);
    load("monsters", monsters);
    load("items", items);
    load("effects", effects);
    load("sounds", sounds);
    load("font", font);
    load("map_width", map_width);
    load("map_height", map_height);
    load("room_placement_attempts", room_placement_attempts);
}
