#pragma once

#include <string>
#include <map>
#include <iostream>

#include "resource.hpp"

typedef enum {
    kInvalid = -1,
    kInt     = 0,
    kUInt    = 1,
    kFloat   = 3,
    kVec2    = 4,
    kVec3    = 5,
    kVec4    = 6,
    kString  = 7,
    kBuffer  = 8
} ePropertyType;

typedef struct {
    void* data = NULL;
    int size = 0;
    int type = 0;
} MaterialProperty;

class Material : public Resource {

    private:
        std::map<std::string, MaterialProperty> _propertyMap;

    public:
        Material(std::string name)
        : Resource(name) {}

        template<typename T>
        bool Add(std::string key, T* data) {
            if (_propertyMap.find(key) != _propertyMap.end()) {
                std::cout << "Error: Material::Add - Property key (" << key << ") already exists" << std::endl;
                return false;
            }
            _propertyMap[key] = {(void*) data, sizeof(T), 0};
            return true;
        }

        template<typename T>
        T* Remove(std::string key) {
            T* temp = NULL;
            try {
                temp = _propertyMap.at(key);
            } catch (std::out_of_range err) {
                std::cout << err.what() << std::endl;
            }
            return temp;
        }

        template<typename T>
        T* Get(std::string key) {
            return (T*)_propertyMap.at(key).data;
        }

        void Clear() {
            for (auto it = _propertyMap.begin(); it != _propertyMap.end(); it++) {
                delete it->second.data;
            }
            _propertyMap.clear();
        }

};