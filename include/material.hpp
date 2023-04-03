#pragma once

#include <string>
#include <map>
#include <iostream>

#include "resource.hpp"

typedef enum {
    kTypeInvalid = 0,
    kTypeFloat   = 1,
    kTypeDouble  = 2,
    kTypeString  = 3,
    kTypeInteger = 4,
    kTypeBuffer  = 5
} ePropertyType;

typedef struct {
    std::string key = "NAME";
    char* data = NULL;
    int components = 0;
    ePropertyType type = kTypeInvalid;
} MaterialProperty;


// add validation that map elements dont exist before assigning
class Material : public Resource {

    private:
        static int id;
        std::map<std::string, int> _idMap;
        std::map<int, MaterialProperty> _propertyMap;

    public:
        Material(std::string name)
        : Resource(name) {}

        void Add(std::string key, int* data, int components) {
            _idMap[key] = id++;
            _propertyMap[_idMap[key]] = {key, (char*)data, components, kTypeInteger};
        }

        void Add(std::string key, float* data, int components) {
            _idMap[key] = id++;
            _propertyMap[_idMap[key]] = {key, (char*)data, components, kTypeFloat};
        }

        void Add(std::string key, double* data, int components) {
            _idMap[key] = id++;
            _propertyMap[_idMap[key]] = {key, (char*)data, components, kTypeDouble};
        }
};

int Material::id = 0;