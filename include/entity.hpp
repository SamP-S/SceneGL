#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "object.hpp"

class Component;
class Transform;

class Entity : public Object {
    private:
        std::vector<Entity*> _children;
        Entity* _parent = nullptr;
        std::string _name = "";
        std::vector<Component*> _components;

    public:
        // public accessor
        Transform* transform;

        Entity(std::string name, Entity* parent);
        ~Entity();

        // JSON
        std::string Type();
        void FromJson(json j);
        json ToJson();
        std::string ToString();

        // name
        std::string GetName();
        void SetName(std::string name);

        // children
        void AddChild(Entity* child);
        int GetNumChildren();
        Entity* GetChild(int index);
        Entity* FindChild(std::string key);
        int FindChildIndex(std::string key);
        int FindChildIndex(Entity* key);
        bool RemoveChild(int index);
        bool RemoveChild(Entity* key);

        // parent
        bool IsRoot();
        Entity* GetParent();
        void SetParent(Entity* parent);

        //components
        int GetNumComponents();

        template<typename T>
        T* GetComponent();

        template<typename T>
        std::vector<T*> GetComponents();

        template<typename T>
        T* GetComponentInChildren();

        template<typename T>
        T* GetComponentInParent();

        template<typename T>
        std::vector<T*> GetComponentsInChildren();

        template<typename T>
        std::vector<T*> GetComponentsInParent();

        template<typename T>
        T* AddComponent();

        template<typename T>
        bool RemoveComponent(T* key);

        
};

#include "entity.tpp"