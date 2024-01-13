#pragma once

#include "component.hpp"
#include "la_extended.h"
using namespace LA;

class PointLight : public Component {
private:
    vec3 _colour = vec3({1.0f, 1.0f, 1.0f});
    float _intensity = 1.0f;
    float _range = 10.0f;

public:
    PointLight(Entity& entity, vec3 colour=vec3({1.0f, 1.0f, 1.0f}), float intensity=1.0f, float range=10.0f) :
        Component(entity),
        _colour(colour),
        _intensity(intensity),
        _range(range) {}

    ~PointLight() {}

    std::string ComponentType() {
        return "PointLight";
    }
    
    vec3 GetColour() {
        return _colour;
    }
    void SetColour(vec3 colour) {
        _colour = colour;
    }

    float GetIntensity() {
        return _intensity;
    }
    void SetIntensity(float intensity) {
        _intensity = intensity;
    }

    float GetRange() {
        return _range;
    }
    void SetRange(float range) {
        _range = range;
    }

};
