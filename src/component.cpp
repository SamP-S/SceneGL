#include "component.h"
#include "entity.hpp"

Component::Component(Entity& entity) :
    entity(entity),
    transform(entity.transform) {}

Component::~Component() {}