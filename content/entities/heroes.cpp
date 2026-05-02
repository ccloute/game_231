#include "heroes.h"

#include "action.h"
#include "closedoor.h"
#include "engine.h"
#include "entity.h"
#include "move.h"
#include "rest.h"
#include "spear.h"
#include "sword.h"
#include "vec.h"

#include <cctype>
#include <string>

namespace Heroes {

namespace {

void add_default_weapons(std::shared_ptr<Entity> hero) {
    hero->add_to_inventory(std::make_shared<Spear>(4));
    hero->add_to_inventory(std::make_shared<Sword>(3));
}

}

void make_knight(std::shared_ptr<Entity> hero) {
    hero->set_sprite("knight");
    hero->set_max_health(10);
    add_default_weapons(hero);
    hero->behavior = behavior;
}

std::unique_ptr<Action> behavior(Engine& engine, Entity& entity) {
    const std::string key = engine.input.get_last_keypress();
    if (key == "Right" || key == "D") {
        return std::make_unique<Move>(Vec{1, 0});
    }
    if (key == "Left" || key == "A") {
        return std::make_unique<Move>(Vec{-1, 0});
    }
    if (key == "Up" || key == "W") {
        return std::make_unique<Move>(Vec{0, 1});
    }
    if (key == "Down" || key == "S") {
        return std::make_unique<Move>(Vec{0, -1});
    }
    if (key == "Space") {
        return std::make_unique<Rest>();
    }
    if (key == "C") {
        return std::make_unique<CloseDoor>();
    }
    if (!key.empty() && std::isdigit(static_cast<unsigned char>(key.at(0)))) {
        entity.select_item(std::stoi(key));
        return nullptr;
    }
    if (key == "R") {
        entity.switch_to_next_item();
        return nullptr;
    }
    return nullptr;
}

}
