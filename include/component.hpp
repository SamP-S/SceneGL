#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

class Component {
    private:
        void* _entity = nullptr;

    public:
        Component(void* entity) :
            _entity(entity) {}
        ~Component() {}

        virtual std::string ComponentType() = 0;

};
