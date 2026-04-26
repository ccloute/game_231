#include "entities.h"

#include "action.h"
#include "entity.h"

void Entities::add(const std::shared_ptr<Entity>& entity) {
    entities.push_back(entity); 
}

void Entities::update() {
    // tell all entities to update
    for (const std::shared_ptr<Entity>& entity : entities) {
        entity->update();
    }
}

bool Entities::take_turn(Engine& engine) {
    remove_dead_entities();
    if (entities.empty()) {
        return false;
    }

    std::shared_ptr<Entity> entity = entities.front();
    
    if (entity->energy < cost_of_turn) {
        advance();
        return true; // made progress
    }
    
    // get the entities next action
    std::unique_ptr<Action> action = entity->take_turn();
    // if entity produced no action, then wait for them by not advancing to next entity
    if (!action) {
        return false;
    }
    
    // perform entity's action and all resulting actions until done
    while (true) {
        Result result = action->perform(engine, entity);

        if (result.succeeded) {
            // action was successful, reduce the entity's energy
            entity->energy %= cost_of_turn;

            // apply any conditions
            entity->conditions.execute(engine);

            // move to next entity
            advance();
            return true;
        }
        else { // action failed
            if (result.next_action) {
                // if there is a subsequent action, set it as the next one to perform
                action = std::move(result.next_action);
            }
            else {
                // action failed and no alternative, don't advance to next entity
                return true;
            }
        }
    }
}

void Entities::advance() {
    if (entities.empty()) {
        return;
    }
    auto entity = entities.front();
    entities.pop_front();
    entities.push_back(entity);
    // give entity some energy for their next turn
    entity->energy += entity->speed;
}

void Entities::remove_dead_entities() {
    std::erase_if(entities, [](const std::shared_ptr<Entity>& entity) { return !entity->alive; });
}
