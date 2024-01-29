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
#include "renderer/components.hpp"
#include "renderer/shader.hpp"
#include "renderer/frame_buffer.hpp"
#include "renderer/material.hpp"

//// TODO:
// Add depth test function

//// NOTES:
// The only state stored with the renderer currently:
//      framebuffer
//      view transform
//      projection transform
//      opengl context specific settings

enum class DrawMode {
    POINTS,
    LINES,
    FILL
};

enum class Winding {
    CW,
    CCW
};

class Renderer : public Module {
protected:
    // rendering state
    std::shared_ptr<FrameBuffer> _frameBuffer = nullptr;
    LA::mat4 _projection = LA::mat4();
    LA::mat4 _view = LA::mat4();

    // opengl context settings
    DrawMode _drawMode = DrawMode::FILL;
    float _pointSize = 5.0f;
    float _lineWidth = 3.0f;
    bool _depthTesting = true;
    bool _culling = true;
    Winding _winding = Winding::CCW;
    LA::vec4 _clearColour = LA::vec4(1.0f);

public:
    std::string GetName() override {
        return "Renderer";
    }
    ModuleType GetType() override {
        return ModuleType::RENDER;
    }

    virtual void SetDrawMode(DrawMode m) = 0;
    virtual DrawMode GetDrawMode() = 0;
    virtual void SetPointSize(float size) = 0;
    virtual float GetPointSize() = 0;
    virtual void SetLineWidth(float width) = 0;
    virtual float GetLineWidth() = 0;
    virtual void SetWinding(Winding winding) = 0;
    virtual Winding GetWinding() = 0;
    virtual void SetClearColour(const LA::vec4& colour) = 0;
    virtual LA::vec4 GetClearColour() = 0;
    virtual void SetCulling(bool enabled) = 0;
    virtual bool IsCulling() = 0;
    virtual void SetDepthTesting(bool enabled) = 0;
    virtual bool IsDepthTesting() = 0;

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
        
protected:
    Renderer() = default;
    ~Renderer() = default;

};