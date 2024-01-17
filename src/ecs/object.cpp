#include "ecs/object.hpp"

ObjId Object::_nextId = 1;

Object::Object() {
    _id = Object::_nextId++;
}

ObjId Object::GetId() {
    return _id;
}

 bool Object::operator==(const Object& cmp) const {
            return _id == cmp._id;
        }

bool Object::operator!=(const Object& cmp) const {
    return _id != cmp._id;
}