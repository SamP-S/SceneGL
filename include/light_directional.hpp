#pragma once

#include "component.hpp"
#include "la_extended.h"
using namespace LA;

class DirectionalLight : public Component {
private:
    vec3 _strength;

public:
    DirectionalLight(Entity& entity, vec3 strength=vec3({1.0f, 1.0f, 1.0f})) :
        Component(entity),
        _strength(strength) {}

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
