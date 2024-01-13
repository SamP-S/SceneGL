#pragma once

#include "component.hpp"
#include "la_extended.h"
using namespace LA;

class DirectionalLight : public Component {
private:
    vec3 _strength;

public:
    DirectionalLight(Entity& entity) :
        Component(entity) {}

    ~DirectionalLight() {}

    std::string ComponentType() {
        return "DirectionalLight";
    }
    
    vec3 GetStrength() {
        return _strength;
    }
    void SetStrength(vec3 strength) {
        _strength = strength;
    }

};