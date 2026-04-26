#include "tile.h"
#include "door.h"

Tile::Tile()
    :type{Type::None}, sprite{}, visible{false}, door{nullptr}, entity{nullptr} {}
    
bool Tile::is_wall() const {
    return type == Type::Wall;
}
    
bool Tile::has_door() const {
    return type == Type::Door;
}

bool Tile::has_item() const {
    return item != nullptr;
}

bool Tile::has_entity() const {
    return entity != nullptr;
}

bool Tile::is_walkable() const {    
    if (is_wall() || has_entity()) {
        return false;
    }
    else if (has_door()) {
        return door->is_open();
    }
    else {
        return true;
    }        
}

bool Tile::is_visible() const {
    return visible;
}

