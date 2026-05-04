#pragma once

#include <memory>

class Engine;
class Entity;
class Action;

namespace Heroes {

void make_knight(std::shared_ptr<Entity> hero);

void make_wizard(std::shared_ptr<Entity> hero);

std::unique_ptr<Action> behavior(Engine& engine, Entity&);

}
