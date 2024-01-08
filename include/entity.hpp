#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "component.h"
#include "resource.h"
#include "resource_manager.hpp"
#include "transform.hpp"

typedef int EntityId;

class Entity {
      
    public:
        Transform transform = Transform();

        Entity(std::string name, Entity* parent) :
            _name(name),
            _parent(parent) {
                _entityId = Entity::_nextId++;
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

        int GetNumComponents() {
            return _components.size();
        }

        template<typename T>
        T* GetComponent() {
            for (auto component : _components) {
                if (dynamic_cast<T*>(component) != nullptr) {
                    return dynamic_cast<T*>(component);
                }
            }
            return nullptr;
        }

        template<typename T>
        std::vector<T*> GetComponents() {
            std::vector<T*> results  = std::vector<T*>();
            for (auto component : _components) {
                if (dynamic_cast<T*>(component) != nullptr) {
                    results.push_back(dynamic_cast<T*>(component));
                }
            }
            return results;
        }

        void AddComponent(Component* component) {
            if (component == nullptr) {
                std::cout << "WARNING (Entity): Trying to add NULL component." << std::endl;
                return;
            }
            _components.push_back(component);
        }

        void RemoveComponent(Component* key) {
            if (key == nullptr) {
                std::cout << "WARNING (Entity): Trying to remove NULL component." << std::endl;
                return;
            }
            auto it = std::find(_components.begin(), _components.end(), key);
            if (it != _components.end()) {
                _components.erase(it);
            }
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
        std::string _name = "";

        EntityId _entityId = 0;
        static EntityId _nextId;

        std::vector<Component*> _components = std::vector<Component*>();
        
};

EntityId Entity::_nextId = 0;