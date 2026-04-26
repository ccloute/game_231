#include "rest.h"
#include "entity.h"

Result Rest::perform(Engine&, std::shared_ptr<Entity> entity) {
    entity->take_damage(-1);
    return success();
}
