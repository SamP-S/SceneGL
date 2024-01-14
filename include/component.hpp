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
    Entity& entity;
    Transform& transform;

    Component(Entity& entity);
    ~Component();
};