#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "json.hpp"
using namespace nlohmann;

class Transform;
class Entity;

class Component {
public:
    Entity& entity;
    Transform& transform;

    Component(Entity& entity);
    ~Component();

    virtual std::string ComponentType() = 0;
    virtual void FromJson(json j) = 0;
    virtual json ToJson() = 0;
    virtual std::string ToString() = 0;
};