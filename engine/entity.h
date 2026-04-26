#pragma once

#include "animatedsprite.h"
#include "vec.h"
#include "sprite.h"
#include "events.h"
#include <memory>
#include <vector>
#include <functional>
#include <iostream>

// forward declarations
class Engine;
class Action;
class Item;
class Event;

constexpr int minimum_speed{1}, default_speed{8}, maximum_speed{64};
constexpr int max_inventory{5};
enum class Team { Hero, Monster };

// base class for all interacting beings
class Entity {
public:
    Entity(Engine& engine, Vec position, Team team);

    // movement
    [[nodiscard]] Vec get_position() const;
    void move_to(Vec position);
    [[nodiscard]] Vec get_direction() const;
    void change_direction(Vec new_direction);
    [[nodiscard]] bool is_visible() const;

    void set_speed(int speed);
    [[nodiscard]] int get_speed() const;

    // functions to be called after move_to is called
    std::vector<std::function<void(Engine& engine, Entity& entity)>> on_move;

    // combat
    void take_damage(int amount);
    void set_max_health(int value);
    [[nodiscard]] std::pair<int, int> get_health() const; // returns health, max_health
    [[nodiscard]] bool is_alive() const;
    void set_team(Team new_team);
    [[nodiscard]] Team get_team() const;

    // managing items within the inventory
    [[nodiscard]] bool is_inventory_full() const;
    void add_to_inventory(std::shared_ptr<Item> item);
    [[nodiscard]] std::shared_ptr<Item> get_current_item() const;
    void switch_to_next_item();
    void select_item(int index);
    void remove_item(Item* item);
    std::shared_ptr<Item> remove_item(int index);
    // returns selected item number and all the items in inventory
    [[nodiscard]] std::pair<int, std::vector<std::shared_ptr<Item>>> get_inventory() const;

    // taking turns
    [[nodiscard]] std::unique_ptr<Action> take_turn();
    std::function<std::unique_ptr<Action>(Engine& engine, Entity& entity)> behavior;

    // drawing
    void set_sprite(const std::string& name);
    void update();
    [[nodiscard]] std::vector<Sprite> get_sprites() const;

    // conditions
    template <typename T, typename... Args>
    std::shared_ptr<T> add_condition(Args&&... args) {
        static_assert(std::is_base_of<Event, T>::value, "T must derive from Event");
        auto event = std::make_shared<T>(std::forward<Args>(args)...);
        if (!event) {
            std::cerr << "Entity::add_condition(): event is nullptr\n";
        }
        conditions.add(event);
        return event;
    }

private:
    Engine& engine;
    AnimatedSprite sprite;
    Vec position, direction{1, 0};

    // health gets reduced by calling take damage
    int health{1}, max_health{1};
    bool alive{true};
    
    // teams can be used to determine who can attack whom
    Team team;

    // speed is energy gain per turn, once an entity has enough energy
    // it can take a turn
    int speed{default_speed}, energy{0};

    // inventory stores up to max_inventory items
    std::vector<std::shared_ptr<Item>> inventory;
    int current_item; // item that entity is currently holding and using
    // visually orient the item based on entity's direction
    void adjust_item_position(std::shared_ptr<Item> item);

    // conditions
    Events conditions;

    friend class Entities;
};

