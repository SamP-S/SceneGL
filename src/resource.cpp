#include "resource.hpp"

int Resource::_nextId = 1;

Resource::Resource(std::string name) :
    _name(name) {
    _id = Resource::_nextId++;
}

ResourceId Resource::GetId() {
    return _id;
}

std::string Resource::GetName() {
    return _name;
}

void Resource::SetName(std::string name) {
    _name = name;
}
