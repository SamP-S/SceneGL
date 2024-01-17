#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "ecs/object.hpp"
#include "ecs/entity.hpp"

class Transform;

class Component : public Object {
private:
    friend class Application;
public:
    Entity* const entity;
    Transform* transform;

    Component(Entity* entity);
    ~Component();

    template<typename T>
    T* GetComponent();

    template<typename T>
    T* GetComponentInChildren();

    template<typename T>
    T* GetComponentInParent();

    template<typename T>
    std::vector<T*> GetComponents();

    template<typename T>
    std::vector<T*> GetComponentsInChildren();

    template<typename T>
    std::vector<T*> GetComponentsInParent();

};

#include "ecs/component.tpp"
