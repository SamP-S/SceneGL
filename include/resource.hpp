#pragma once

#include <string>

typedef int ResourceId;

class Resource {

    private:
        std::string _name;
        ResourceId _id;

        static int _nextId;

    public:

        Resource(std::string name) {
            _name = name;
            _id = Resource::_nextId++;
        }

        ResourceId GetId() {
            return _id;
        }

        std::string GetName() {
            return _name;
        }

        void SetName(std::string name) {
            _name = name;
        }

};

int Resource::_nextId = 1;