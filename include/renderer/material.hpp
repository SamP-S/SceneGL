#pragma once

#include <string>
#include <map>
#include <iostream>

#include "tai/tai.hpp"

class Material : public Asset {

    private:
        vec4 _baseColour = vec4(1.0f);
        float _metallic = 1.0f;
        float _roughness = 1.0f;

    public:
        Material(std::string name)
        : Asset(name) {}

        Material(std::string name, vec4 baseColour, float metallic, float roughness)
        : Asset(name),
        _baseColour(baseColour),
        _metallic(metallic),
        _roughness(roughness) {}

        std::string Type() {
            return "Material";
        }
        void FromJson(json j) {
            json colour = j["baseColour"];
            _baseColour = vec4({colour["r"], colour["g"], colour["b"], colour["a"]});
            _metallic = j["metallic"];
            _roughness = j["roughness"];
            return;
        }
        json ToJson() {
            json j;
            j["baseColour"] = {
                {"r", _baseColour.x}, {"g", _baseColour.y}, 
                {"b", _baseColour.z}, {"a", _baseColour.w}
            };
            j["metallic"] = _metallic;
            j["roughness"] = _roughness;
            return j;
        }
        std::string ToString() {
            return "Material";
        }
};
