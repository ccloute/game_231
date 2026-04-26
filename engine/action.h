#pragma once

#include <memory>

// forward declarations
class Engine;
class Action;
class Entity;


// the result of performing an Action
class Result {
public:
    bool succeeded{false};
    std::unique_ptr<Action> next_action{nullptr}; // allows chaining of actions
};


// base class for all actions
class Action {
public:
    virtual ~Action() = default;

    // override perform in a derived class
    virtual Result perform(Engine& engine, std::shared_ptr<Entity> entity) = 0;
};
    

// Helper functions for generating results. You will use these inside
// Perform() of derived classes to signal whether an action was
// successful, failed, or that the entity needs to perform an
// alternative action instead.

// return success() if the entity completed the action and its turn is
// over
Result success();

// return failure() if the action cannot be performed, and you'd like
// the entity to have another turn
Result failure();


// return alternative(another_action{}) to substitute the entity's
// current action for another
template <typename T>
Result alternative(const T& action) {
    return {false, std::make_unique<T>(action)};
}

