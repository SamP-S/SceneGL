#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>

// internal libs
#include "la_extended.h"
#include "renderer/mesh.hpp"
#include "renderer/material.hpp"

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
    LA::vec3 colour = LA::vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;

    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLightComponent&) = default;
};

#define POINT_LIGHT_MAX 16
struct PointLightComponent {
    LA::vec3 colour = LA::vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;
    float range = 10.0f;

    PointLightComponent() = default;
    PointLightComponent(const PointLightComponent&) = default;
};

#define SPOT_LIGHT_MAX 4
struct SpotLightComponent {
    LA::vec3 colour = LA::vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;
    float cutOff = 12.5f;
    float outerCutOff = 15.0f;

    SpotLightComponent() = default;
    SpotLightComponent(const SpotLightComponent&) = default;
};

// Note: will keep assets alive even if it has been removed from AssetManager until reassigned
struct MeshRendererComponent {
    std::shared_ptr<Mesh> mesh = nullptr;
    std::shared_ptr<Material> material = nullptr;

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
};
