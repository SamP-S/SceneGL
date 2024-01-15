#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "object.hpp"

class Transform;
class Entity;

class Component : public Object {
public:
    const Entity& entity;
    Transform& transform;

    Component(Entity& entity);
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

#include "component.tpp"
