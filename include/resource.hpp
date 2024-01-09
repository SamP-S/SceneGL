#pragma once

#include <string>

typedef int ResourceId;

class Resource {

    private:
        std::string _name;
        ResourceId _id;
        static int _nextId;

    public:
        Resource(std::string name);
        
        ResourceId GetId();
        std::string GetName();
        void SetName(std::string name);
};