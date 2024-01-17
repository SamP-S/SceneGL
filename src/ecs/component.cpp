#include "ecs/component.hpp"
#include "ecs/entity.hpp"

Component::Component(Entity* entity) :
    entity(entity),
    transform(entity->transform) {}

Component::~Component() {}