#pragma once
#include <string>

class Resource {

    private:
        std::string _name;

    public:

        Resource(std::string name) {
            _name = name;
        }

        std::string GetName() {
            return _name;
        }

};