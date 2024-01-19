#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
// internal libs
#include "la_extended.h"

struct CoreComponent {
    std::string name;
    bool active = true;

    CoreComponent() = default;
    CoreComponent(const CoreComponent&) = default;
    CoreComponent(const std::string& pName)
        : name(pName) {}
};

struct TransformComponent {
    LA::vec3 position  = LA::vec3(0.0f);
    LA::vec3 rotation  = LA::vec3(0.0f);
    LA::vec3 scale     = LA::vec3(1.0f);

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const LA::vec3& pos)
        : position(pos) {}


    LA::mat4 GetTransform() {
        return LA::Transformation(position, rotation, scale);
    }

    LA::vec3 GetForward() {
        LA::mat4 total = GetTransform();
        return LA::vec3({ total[2][0], total[2][1], total[2][2] });
    }

    LA::vec3 GetRight() {
        LA::mat4 total = GetTransform();
        return LA::vec3({ total[0][0], total[0][1], total[0][2] });
    }

    LA::vec3 GetUp() {
        LA::mat4 total = GetTransform();
        return LA::vec3({ total[1][0], total[1][1], total[1][2] });
    }
    
};

struct CameraComponent {
    float fov = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    uint32_t width = 800;
    uint32_t height = 600;


    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;

    LA::mat4 GetProjection() {
        return LA::Perspective(fov, (float)width / (float)height, near, far);
    }
};

#define DIRECTIONAL_LIGHT_MAX 4
struct DirectionalLightComponent {
    LA::vec3 colour = vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;

    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLightComponent&) = default;
};

#define POINT_LIGHT_MAX 16
struct PointLightComponent {
    LA::vec3 colour = vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;
    float range = 10.0f;

    PointLightComponent() = default;
    PointLightComponent(const PointLightComponent&) = default;
};

struct MeshRendererComponent {
    uint32_t mesh = 0;

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
};
