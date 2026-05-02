#include "monsters.h"

#include "action.h"
#include "engine.h"
#include "entity.h"
#include "mace.h"
#include "move.h"
#include "randomness.h"
#include "rest.h"
#include "spear.h"
#include "sword.h"
#include "vec.h"
#include "wander.h"

namespace Monsters {

namespace {

std::unique_ptr<Action> behavior(Engine& engine, Entity& entity) {
    if (entity.is_visible() && engine.hero) {
        const auto path = engine.dungeon.calculate_path(entity.get_position(),
                                                        engine.hero->get_position());
        if (path.size() > 1) {
            const Vec direction = path.at(1) - path.at(0);
            return std::make_unique<Move>(direction);
        }
    }
    if (probability(66)) {
        return std::make_unique<Wander>();
    }
    return std::make_unique<Rest>();
}

}

void make_orc_masked(std::shared_ptr<Entity> monster) {
    monster->set_sprite("orc_masked");
    monster->set_max_health(12);
    monster->add_to_inventory(std::make_shared<Mace>(3));
    monster->behavior = behavior;
}

void make_skeleton(std::shared_ptr<Entity> monster) {
    monster->set_sprite("skeleton");
    monster->set_max_health(8);
    monster->add_to_inventory(std::make_shared<Sword>(2));
    monster->behavior = behavior;
}

void make_goblin(std::shared_ptr<Entity> monster) {
    monster->set_sprite("goblin");
    monster->set_max_health(6);
    monster->add_to_inventory(std::make_shared<Spear>(2));
    monster->behavior = behavior;
}

}
