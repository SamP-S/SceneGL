#pragma once
#include <vector>
#include <string>

#include "resource.hpp"
#include "transform.hpp"

class Object : public Resource {

    int _meshId = -1;
        
    public:
        Transform trans = Transform();

        Object(std::string name, Object* parent) :
            Resource(name),
            _parent(parent) {};

        Object(std::string name, Object* parent, int meshId) :
            Object(name, parent) {
                _meshId = meshId;
            };

        void AddChild(Object* childPtr) {
            _children.push_back(childPtr);
        }

        Object* GetChild(int index) {
            return _children.at(index);
        }

        Object* RemoveChild(int index) {
            Object* childPtr = GetChild(index);
            _children.erase(_children.begin() + index);
            return childPtr;
        }

        void SetParent(Object* parent) {
            _parent = parent;
        }

        int GetMesh() {
            return _meshId;
        }

        void SetMesh(int meshId) {
            this->_meshId = meshId;
        }

    private:
        std::vector<Object*> _children = std::vector<Object*>();
        Object* _parent = NULL;
};