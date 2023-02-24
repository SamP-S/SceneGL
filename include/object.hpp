#include <vector>

#include "transform.hpp"

class Object {

    public:
        Transform trans = Transform();

        Object(Object* parent) :
            _parent(parent) {};


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

    private:
        std::vector<Object*> _children = std::vector<Object*>();
        Object* _parent = NULL;
};