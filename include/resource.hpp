#pragma once

#include <string>

#include "object.hpp"


class Resource : public Object {

    private:
        std::string _name;

    public:
        Resource(std::string name);
        
        std::string GetName();
        void SetName(std::string name);
};