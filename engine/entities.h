#pragma once

#include <memory>
#include <deque>

// forward declarations
class Engine;
class Entity;

// manages a collection of entities
class Entities {
public:
    void add(const std::shared_ptr<Entity>& entity);
    void update();
    bool take_turn(Engine& engine);

    void advance();

    // for iterating over entities
    auto begin() { return std::begin(entities); }
    [[nodiscard]] auto begin() const { return std::begin(entities); }
    auto end() { return std::end(entities); }
    [[nodiscard]] auto end() const { return std::end(entities); }

private:
    const int cost_of_turn{8}; // energy need for taking a turn
    std::deque<std::shared_ptr<Entity>> entities;
    void remove_dead_entities();
};

