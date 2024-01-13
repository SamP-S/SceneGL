#pragma once

#include <string>

typedef int ObjId;

class Object {

    private:
        ObjId _id;
        static ObjId _nextId;

    public:
        Object();
        ObjId GetId();
};