#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

#include "transform.hpp"

class Entity;

class Component {
public:
    Entity& entity;
    Transform& transform;

    Component(Entity& entity);
    ~Component();

    virtual std::string ComponentType() = 0;
};