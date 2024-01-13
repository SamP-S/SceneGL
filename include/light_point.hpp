#pragma once

#include "component.hpp"
#include "la.h"
using namespace LA;

class PointLight : public Component {
private:
    vec3 _strength;

public:
    PointLight(Entity& entity) :
        Component(entity) {}

    ~PointLight() {}

    std::string ComponentType() {
        return "PointLight";
    }
    
    float GetStrength() {
        return _strength;
    }
    void SetStrength(vec3 strength) {
        _strength = strength;
    }

};
