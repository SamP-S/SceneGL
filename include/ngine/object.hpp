#pragma once

#include <cstdint>

using ObjectId = int32_t;

struct Object {
    private:
        inline static ObjectId _nextId = 1;

    public:
        // DO NOT MODIFY STORED VALUE
        ObjectId id;
        
        Object()
            : id(Object::_nextId++) {}

        operator bool() const {
            return id != 0;
        }

        bool operator==(const Object& cmp) const {
            return id == cmp.id;
        }

        bool operator!=(const Object& cmp) const {
            return id != cmp.id;
        }
};