#include "engine.h"
#include "action.h"
#include "entity.h"
#include "heroes.h"
#include "monsters.h"

int main() {
    try {
        Settings settings{"settings.txt"};
        Engine engine{settings};

        std::shared_ptr<Entity> hero = engine.create_hero();
        Heroes::make_knight(hero);

        for (int i = 0; i < 4; ++i) {
            Monsters::make_orc_masked(engine.create_monster());
        }
        for (int i = 0; i < 4; ++i) {
            Monsters::make_skeleton(engine.create_monster());
        }
        for (int i = 0; i < 4; ++i) {
            Monsters::make_goblin(engine.create_monster());
        }

        engine.run();
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}