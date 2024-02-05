#pragma once

// includes
#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#include "core/module.hpp"

#include "la_extended.h"
#include "renderer/renderer_api.hpp"
#include "renderer/components.hpp"
#include "renderer/shader.hpp"
#include "renderer/frame_buffer.hpp"
#include "renderer/material.hpp"
#include "renderer/context.hpp"


class Renderer : public Module {
protected:
    // rendering state
    std::shared_ptr<FrameBuffer> _frameBuffer = nullptr;
    LA::mat4 _projection = LA::mat4();
    LA::mat4 _view = LA::mat4();

public:
    std::string GetName() override {
        return "Renderer";
    }
    ModuleType GetType() override {
        return ModuleType::RENDER;
    }

    std::shared_ptr<Context> context = nullptr;

    // set null to draw directly to screen
    virtual void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) = 0;
    virtual std::shared_ptr<FrameBuffer> GetFrameBuffer() = 0;

    virtual void SetProjection(const LA::mat4& proj) = 0;
    virtual LA::mat4 GetProjection() = 0;

    virtual void SetView(const LA::mat4& view) = 0;
    virtual LA::mat4 GetView() = 0;

    virtual void Clear() = 0;
    
    virtual void RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const LA::mat4& transform) = 0;

    // delete copy and assign operators should always get instance from class::instance func
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    static Renderer& Instance();
        
protected:
    Renderer() = default;
    ~Renderer() = default;

};