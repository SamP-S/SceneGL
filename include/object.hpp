#pragma once

#include <iostream>
#include <string>

#include "json.hpp"
using namespace nlohmann;

typedef int ObjId;

class Object {

    private:
        ObjId _id;
        static ObjId _nextId;

    public:
        Object();
        ObjId GetId();

        virtual std::string ComponentType() = 0;
        virtual void FromJson(json j) = 0;
        virtual json ToJson() = 0;
        virtual std::string ToString() = 0;
};