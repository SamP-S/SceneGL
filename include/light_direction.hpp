#pragma once

#include "component.hpp"
#include "la.h"
using namespace LA;

class DirectionLight : public Component {
private:
    vec3 _direction;
    vec3 _strength;

public:
    DirectionLight(Entity& entity) :
        Component(entity) {}

    ~DirectionLight() {}

    std::string ComponentType() {
        return "DirectionLight";
    }
    
    float GetStrength() {
        return _strength;
    }
    void SetStrength(vec3 strength) {
        _strength = strength;
    }

};
