#pragma once

#include <vector>
#include <string>

#include "resource_manager.hpp"
#include "resource.hpp"
#include "transform.hpp"

typedef int EntityId;

class Entity {
      
    public:
        Transform trans = Transform();

        Entity(std::string name, Entity* parent) :
            _name(name),
            _parent(parent) {
                _entityId = Entity::_nextId++;
            };

        Entity(std::string name, Entity* parent, int meshId) :
            Entity(name, parent) {
                _meshId = meshId;
            };

        std::string GetName() {
            return _name;
        }

        void SetName(std::string name) {
            _name = name;
        }

        void AddChild(Entity* child) {
            _children.push_back(child);
        }

        int GetNumChildren() {
            return _children.size();
        }

        Entity* GetChild(int index) {
            return _children.at(index);
        }

        Entity* FindChild(std::string key) {
            return GetChild(FindChildIndex(key));
        }

        int FindChildIndex(std::string key) {
            for (size_t i = 0; i < _children.size(); i++) {
                const auto& child = _children[i];
                if (child->GetName().compare(key) == 0) {
                    return i;
                }
            }
            return -1;
        }

        int FindChildIndex(Entity* key) {
            for (size_t i = 0; i < _children.size(); i++) {
                const auto& child = _children[i];
                if (child == key) {
                    return i;
                }
            }
            return -1;
        }

        bool IsRoot() {
            return _parent == NULL;
        }

        Entity* GetParent() {
            return _parent;
        }

        void SetParent(Entity* parent) {
            if (_parent != NULL) {
                _parent->RemoveChild(this);
            }
            _parent = parent;
        }

        int GetMesh() {
            return _meshId;
        }

        void SetMesh(int meshId) {
            std::cout << "SetMesh: " << meshId << std::endl;
            this->_meshId = meshId;
        }

    private:

        bool RemoveChild(int index) {
            if (index < 0 || index >= GetNumChildren()) {
                std::cout << "WARNING (Entity): Trying to remove child of invalid index." << std::endl;
                return false;
            }
            _children.erase(_children.begin() + index);
            return true;
        }

        bool RemoveChild(Entity* key) {
            return RemoveChild(FindChildIndex(key));
        }

        std::vector<Entity*> _children = std::vector<Entity*>();
        Entity* _parent = NULL;
        int _meshId = 0;
        std::string _name = "";
        EntityId _entityId = 0;

        static EntityId _nextId;
        
};

EntityId Entity::_nextId = 0;