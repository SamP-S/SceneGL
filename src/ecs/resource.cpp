#include "ecs/resource.hpp"

Resource::Resource(std::string name) :
    Object(),
    _name(name) {}

std::string Resource::GetName() {
    return _name;
}

void Resource::SetName(std::string name) {
    _name = name;
}
