#pragma once

#include "ecs/component.hpp"
#include "la_extended.h"
using namespace LA;

#define DIRECTIONAL_LIGHT_MAX 4

class DirectionalLight : public Component {
private:
    vec3 _colour = vec3({1.0f, 1.0f, 1.0f});
    float _intensity = 1.0f;

public:
    DirectionalLight(Entity* entity, vec3 colour=vec3({1.0f, 1.0f, 1.0f}), float intensity=1.0f) :
        Component(entity),
        _colour(colour),
        _intensity(intensity) {}

    ~DirectionalLight() {}

    std::string Type() {
        return "DirectionalLight";
    }

    void FromJson(json j) {
        json colour = j["colour"];
        _colour = vec3({colour["r"], colour["g"], colour["b"]});
        _intensity = j["intensity"];
    }

    json ToJson() {
        json j;
        j["directionalLight"]["colour"] = {{"r", _colour.x}, {"g", _colour.y}, {"b", _colour.z}};
        j["directionalLight"]["intensity"] = _intensity;
        return j;
    }

    std::string ToString() {
        std::string s = "DirectionalLight(";
        s += "r=" + std::to_string(_colour[0]) + ", ";
        s += "g=" + std::to_string(_colour[1]) + ", ";
        s += "b=" + std::to_string(_colour[2]) + ", ";
        s += "intensity=" + std::to_string(_intensity) + ")";
        return s;
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

};
