#include "use.h"
#include "entity.h"
#include "item.h"

Result Use::perform(Engine& engine, std::shared_ptr<Entity> entity) {
    auto item = entity->get_current_item();
    if (item) {
        item->use(engine, *entity);
        return success();
    }
    return failure();
}