#pragma once

#include <vector>
#include <string>

#include "resource_manager.hpp"
#include "resource.hpp"
#include "transform.hpp"

typedef int EntityId;

class Entity : public Resource {

    int _meshId = -1;
        
    public:
        Transform trans = Transform();

        Entity(std::string name, EntityId parent) :
            Resource(name),
            _parent(parent) {};

        Entity(std::string name, EntityId parent, int meshId) :
            Entity(name, parent) {
                _meshId = meshId;
            };

        void AddChild(EntityId childId) {
            _children.push_back(childId);
        }

        int GetNumChildren() {
            return _children.size();
        }

        EntityId GetChild(int index) {
            return _children.at(index);
        }

        EntityId RemoveChild(int index) {
            EntityId childId = GetChild(index);
            _children.erase(_children.begin() + index);
            return childId;
        }

        void SetParent(EntityId parent) {
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
        std::vector<EntityId> _children = std::vector<EntityId>();
        EntityId _parent = -1;
};

ResourceManager<Entity> resourceEntities = ResourceManager<Entity>();