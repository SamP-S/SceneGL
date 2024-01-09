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

        Entity(std::string name, Entity* parent);

        std::string GetName();
        void SetName(std::string name);

        void AddChild(Entity* child);
        int GetNumChildren();
        Entity* GetChild(int index);
        Entity* FindChild(std::string key);
        int FindChildIndex(std::string key);
        int FindChildIndex(Entity* key);

        bool IsRoot();
        Entity* GetParent();
        void SetParent(Entity* parent);

        int GetNumComponents();

        template<typename T>
        T* GetComponent();

        template<typename T>
        std::vector<T*> GetComponents();

        void AddComponent(Component* component);
        void RemoveComponent(Component* key);

    private:

        bool RemoveChild(int index);
        bool RemoveChild(Entity* key);

        std::vector<Entity*> _children = std::vector<Entity*>();
        Entity* _parent = NULL;
        std::string _name = "";

        EntityId _entityId = 0;
        static EntityId _nextId;

        std::vector<Component*> _components = std::vector<Component*>();
        
};

#include "entity.tpp"