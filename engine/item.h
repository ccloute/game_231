#pragma once

#include <string>

#include "sprite.h"

// forward declarations
class Engine;
class Entity;

class Item {
public:
    // name corresponds to a sprite in items.txt
    explicit Item(std::string name);
    virtual ~Item() = default;

    // override what happens when the weapon is used in game
    // 1) use the item on yourself
    virtual void use(Engine& engine, Entity& owner);
    // 2) use the item on someone else
    virtual void use(Engine& engine, Entity& attacker, Entity& defender);

    // override if you want something to happen when an entity interacts
    // with this item, e.g. touching it, picking it up
    virtual void interact(Engine& engine, Entity& entity);

    std::string name;
    Sprite sprite;
};
