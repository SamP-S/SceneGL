#pragma once

#include <string>
#include <map>
#include <iostream>
#include <variant>
#include <type_traits>

#include "ecs/asset.hpp"
#include "la_extended.h"
#include "renderer/renderer_api.hpp"
#include "renderer/shader.hpp"

//// TODO:
// Integrate more thoroughly with shader
// Should not allow for properties that do not occur in shader
// Should not allow any properties if no shader attached
// Poternially have support methods for checking what variables names are/aren't in the shader
// Potentially remove support getters/setters

using GLSLType = std::variant<
    bool, // for GLSL bool
    int, // for GLSL int
    float, // for GLSL float
    uint32_t, // for GLSL uint
    LA::vec2, // for GLSL vec2
    LA::vec3, // for GLSL vec3
    LA::vec4, // for GLSL vec4
    LA::mat2, // for GLSL mat2
    LA::mat3, // for GLSL mat3
    LA::mat4  // for GLSL mat4
>;

template <class T, class... Ts>
struct is_in_variant;

template <class T, class... Ts>
struct is_in_variant<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template <class T, class... Ts>
inline constexpr bool is_in_variant_v = is_in_variant<T, Ts...>::value;

class Material : public Asset {
public:
    std::map<std::string, GLSLType> properties;
    std::shared_ptr<Shader> shader = nullptr;

    Material(std::string name="Material")
        : Asset(name) {}

    virtual bool IsUsable() const = 0;
    virtual void Bind() = 0;

    void SetProperty(const std::string& key, GLSLType value) {
        properties[key] = value;
    }

    void RemoveProperty(const std::string& key) {
        properties.erase(key);
    }

    template<typename T>
    T& GetProperty(const std::string& key) const {
        static_assert(is_in_variant_v<T>, "T must be a type in GLSLType");
        auto it = properties.find(key);
        if (it != properties.end()) {
            try {
                return std::get<T>(it->second);
            } catch (const std::bad_variant_access& e) {
                std::cout << "WARNING (Material): " << key << " is not of type " << typeid(T).name() << std::endl;
                std::cout << e.what() << std::endl; 
            }
            return 0;
        }
        // Handle case where property is not found
        std::cout << "WARNING (Material): Can't find property matching given key @ " << key << std::endl;
        return 0;
    }

    static std::shared_ptr<Material> Create(const std::string& name);

};
