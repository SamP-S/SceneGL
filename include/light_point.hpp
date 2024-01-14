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

    std::string Type() {
        return "PointLight";
    }

    void FromJson(json j) {
        json colour = j["colour"];
        _colour = vec3({colour["r"], colour["g"], colour["b"]});
        _intensity = j["intensity"];
        _range = j["range"];
    }

    json ToJson() {
        json j;
        j["colour"] = {{"r", _colour.x}, {"g", _colour.y}, {"b", _colour.z}};
        j["intensity"] = _intensity;
        j["range"] = _range;
        return j;
    }

    std::string ToString() {
        std::string s = "DirectionalLight(";
        s += "r=" + std::to_string(_colour[0]) + ", ";
        s += "g=" + std::to_string(_colour[1]) + ", ";
        s += "b=" + std::to_string(_colour[2]) + ", ";
        s += "intensity=" + std::to_string(_intensity) + ", ";
        s += "range=" + std::to_string(_range) + ")";
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

    float GetRange() {
        return _range;
    }
    void SetRange(float range) {
        _range = range;
    }

};
