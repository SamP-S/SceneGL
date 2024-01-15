#include "component.hpp"
#include "entity.hpp"

Component::Component(Entity* entity) :
    entity(entity),
    transform(entity->transform) {}

Component::~Component() {}