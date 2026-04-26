#pragma once

#include "sprite.h"

class Tile;

// A door that can be opened and closed
class Door {
public:
    Door(Tile& tile, bool is_horizontal, const Sprite& horizontal, const Sprite& vertical);
    [[nodiscard]] bool is_open() const;
    void open();
    void close();
    [[nodiscard]] const Sprite& get_sprite() const;
    [[nodiscard]] const Tile& get_tile() const;
    
private:
    Tile& tile;
    bool open_status;
    bool is_horizontal;
    Sprite horizontal, vertical;
};

