#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "component.hpp"
#include "resource.hpp"
#include "resource_manager.hpp"
#include "transform.hpp"

typedef int EntityId;

class Entity {
      
    public:
        Transform transform = Transform(*this);

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

        template<typename T>
        T* GetComponentInChildren();

        template<typename T>
        std::vector<T*> GetComponentsInChildren();

        void AddComponent(Component* component);
        void RemoveComponent(Component* key);

    private:

        bool RemoveChild(int index);
        bool RemoveChild(Entity* key);

        std::vector<Entity*> _children;
        Entity* _parent = nullptr;
        std::string _name = "";

        EntityId _entityId = 0;
        static EntityId _nextId;

        std::vector<Component*> _components;
        
};

#include "entity.tpp"