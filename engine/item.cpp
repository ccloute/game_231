#include "item.h"

#include <utility>

Item::Item(std::string name)
    :name{std::move(name)} {}

void Item::use(Engine&, Entity&) {}

void Item::use(Engine&, Entity&, Entity&) {}

void Item::interact(Engine&, Entity&) {}