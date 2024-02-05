#pragma once

#include "renderer/context.hpp"
#include "platform/opengl/opengl.hpp"

class OpenGLContext : public Context {
public:
    OpenGLContext();
    ~OpenGLContext() = default;

    void SetDrawMode(DrawMode m) override;
    DrawMode GetDrawMode() override;
    void SetPointSize(float size) override;
    float GetPointSize() override;
    void SetLineWidth(float width) override;
    float GetLineWidth() override;
    void SetWinding(Winding winding) override;
    Winding GetWinding() override;
    void SetClearColour(const LA::vec4& colour) override;
    LA::vec4 GetClearColour() override;
    void SetCulling(bool enabled) override;
    bool IsCulling() override;
    void SetDepthTesting(bool enabled) override;
    bool IsDepthTesting() override;

    // delete copy and assign operators should always get instance from class::instance func
    OpenGLContext(const OpenGLContext&) = delete;
    OpenGLContext& operator=(const OpenGLContext&) = delete;
    
};