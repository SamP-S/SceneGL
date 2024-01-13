#include "object.hpp"

ObjId Object::_nextId = 1;

Object::Object() {
    _id = Object::_nextId++;
}

ObjId Object::GetId() {
    return _id;
}
