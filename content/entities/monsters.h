#pragma once

#include <memory>

class Entity;

namespace Monsters {

void make_orc_masked(std::shared_ptr<Entity> monster);
void make_skeleton(std::shared_ptr<Entity> monster);
void make_goblin(std::shared_ptr<Entity> monster);

}
