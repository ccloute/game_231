#include "entity.h"

#include <sstream>
#include <algorithm>
#include "action.h"
#include "engine.h"
#include "item.h"

Entity::Entity(Engine& engine, Vec position, Team team)
    : engine{engine},
      position{position},
      team{team},
      inventory(max_inventory),
      current_item{0}{

    // place entity onto its dungeon tile
    Tile& tile = engine.dungeon.get_tile(position);
    if (tile.entity == nullptr) {
        tile.entity = this;
    } else {  // an entity is already on this tile, throw error
        std::stringstream ss{"An entity is already on tile: "};
        ss << position;
        throw std::runtime_error(ss.str());
    }
}

void Entity::change_direction(Vec new_direction) {
    direction = new_direction;
    if (new_direction.x == 1) {
        sprite.flip(false);
    } else if (new_direction.x == -1) {
        sprite.flip(true);
    }
    adjust_item_position(get_current_item());
}

void Entity::move_to(Vec new_position) {
    Tile& old_tile = engine.dungeon.get_tile(position);
    Tile& new_tile = engine.dungeon.get_tile(new_position);
    std::swap(old_tile.entity, new_tile.entity);
    position = new_position;
    for (const auto& func : on_move) {
        func(engine, *this);
    }
}

Vec Entity::get_position() const {
    return position;
}

Vec Entity::get_direction() const {
    return direction;
}

bool Entity::is_visible() const {
    // an entity is visible if its tile is visible
    const Tile& tile = engine.dungeon.get_tile(position);
    return tile.is_visible();
}


void Entity::set_speed(int speed) {
    speed = std::clamp(speed, minimum_speed, maximum_speed);
}

int Entity::get_speed() const {
    return speed;
}

void Entity::take_damage(int amount) {
    health -= amount;
    health = std::clamp(health, 0, max_health);
    if (health == 0) {
        alive = false;
    }
}

void Entity::set_max_health(int value) {
    max_health = health = value;
    alive = health > 0;
}

std::pair<int, int> Entity::get_health() const {
    return {health, max_health};
}

bool Entity::is_alive() const {
    return alive;
}

void Entity::set_team(Team new_team) {
    team = new_team;
}

Team Entity::get_team() const {
    return team;
}

bool Entity::is_inventory_full() const {
    auto empty_slots = std::count(std::begin(inventory), std::end(inventory), nullptr);
    return empty_slots == 0;
}

void Entity::add_to_inventory(std::shared_ptr<Item> item) {
    auto it = std::find(std::begin(inventory), std::end(inventory), nullptr);
    if (it != inventory.end()) { // empty slot found
        item->sprite = engine.graphics.get_sprite(item->name);
        *it = std::move(item);
        adjust_item_position(*it);
    }
}

std::shared_ptr<Item> Entity::get_current_item() const {
    std::shared_ptr<Item> item = inventory.at(current_item);
    if (item) {
        return item;
    }
    else {  // return empty item
        return std::make_shared<Item>("none");
    }
}

void Entity::switch_to_next_item() {
    current_item = (current_item + 1) % max_inventory;
    adjust_item_position(get_current_item());
}

void Entity::select_item(int index) {
    // assumes 1-5 is passed in, convert to an index
    --index;
    
    // ensure valid index within inventory
    if (0 <= index && index < max_inventory) {
        current_item = index;
        adjust_item_position(get_current_item());
    }
}

void Entity::remove_item(Item* item) {
    auto it = std::find_if(std::begin(inventory), std::end(inventory),
                        [=](std::shared_ptr<Item>& i) {
                            return item == i.get();
                        });

    if (it != inventory.end()) { // found the item
        *it = nullptr;
    }
}

std::shared_ptr<Item> Entity::remove_item(int index) {
    if (0 <= index && index < max_inventory) {
        std::shared_ptr<Item> item = std::move(inventory.at(index));
        inventory.at(index) = nullptr;
        return item;
    }
    else {
        return nullptr;
    }
}

std::pair<int, std::vector<std::shared_ptr<Item>>> Entity::get_inventory() const {
    return {current_item, inventory};
}

std::unique_ptr<Action> Entity::take_turn() {
    if (behavior) {
        return behavior(engine, *this);
    }
    return nullptr;
}

void Entity::set_sprite(const std::string& name) {
    sprite = engine.graphics.get_animated_sprite(name, 1, true);
}

void Entity::update() {
    sprite.update();
}

std::vector<Sprite> Entity::get_sprites() const {
    std::shared_ptr<Item> item = get_current_item();
    if (item) {
        return {item->sprite, sprite.get_sprite()};
    }
    else {
        return {sprite.get_sprite()};
    }
}


void Entity::adjust_item_position(std::shared_ptr<Item> item) {
    if (item) {
        if (direction.x == 1) {
            item->sprite.flip = false;
            item->sprite.shift.x = sprite.get_sprite().size.x / 4;
            item->sprite.angle = 20;
        } else if (direction.x == -1) {
            item->sprite.flip = true;
            item->sprite.shift.x = -3*sprite.get_sprite().size.x / 4;
            item->sprite.angle = -20;
        }
    }
}

