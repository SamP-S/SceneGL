#pragma once

// std libs
#include <string>
#include <cassert>
#include <cstdint>
#include <vector>
// internal libs
#include "ngine/ngine.hpp"
#include "la_extended.h"

struct CameraComponent : public Ngine::Object {
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
struct DirectionalLightComponent : public Ngine::Object {
    LA::vec3 colour = LA::vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;

    DirectionalLightComponent() = default;
    DirectionalLightComponent(const DirectionalLightComponent&) = default;
};

#define POINT_LIGHT_MAX 16
struct PointLightComponent : public Ngine::Object {
    LA::vec3 colour = LA::vec3({1.0f, 1.0f, 1.0f});
    float intensity = 1.0f;
    float range = 10.0f;

    PointLightComponent() = default;
    PointLightComponent(const PointLightComponent&) = default;
};

struct MeshRendererComponent : public Ngine::Object {
    ObjectId mesh = 0;
    ObjectId shader = 0;

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
};
