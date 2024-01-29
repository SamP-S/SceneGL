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

#include "renderer/renderer.hpp"
#include "platform/opengl/opengl.hpp"
#include "platform/opengl/opengl_shader.hpp"
#include "platform/opengl/opengl_mesh.hpp"


//// TODO:
// no method for glViewport(0, 0, width, height);


//// NOTES:
// switching shader will need all uniforms reassigned
// switching framebuffer will ONLY affect where pixels are drawn to
// switching material/proj/view will ONLY affect bound shader

class OpenGLRenderer : public Renderer {
public:
    void Boot() override {
        std::cout << "DEBUG (OpenGLRenderer): Boot." << std::endl;
        SetPointSize(_pointSize);
        SetLineWidth(_lineWidth);
        SetWinding(_winding);
        SetClearColour(_clearColour);
        SetCulling(true);
        SetDepthTesting(true);
    }

    void Shutdown() override {
        std::cout << "DEBUG (OpenGLRenderer): Shutdown." << std::endl;
    }
    
    void SetDrawMode(DrawMode drawMode) override {
        _drawMode = drawMode;
        if (m == DrawMode::POINTS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        else if (m == DrawMode::LINES)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    DrawMode GetDrawMode() override {
        return _drawMode;
    }

    void SetPointSize(float size) override {
        _pointSize = size;
        glPointSize(size);
    }
    float GetPointSize() override {
        return _pointSize;
    }

    void SetLineWidth(float width) override {
        _width = width;
        glLineWidth(width);
    }
    float GetLineWidth() override {
        return _lineWidth;
    }

    void SetWinding(Winding winding) override {
        _winding = winding;
        if (winding == Winding::CW)
            glFrontFace(GL_CW);
        else
            glFrontFace(GL_CCW);
    }
    Winding GetWinding() override {
        return _winding;
    }

    void SetClearColour(const LA::vec4& c) override {
        _clearColour = c;
        glClearColor(c.r, c.g, c.b, c.a);
    }
    LA::vec4 GetClearColour() override {
        return _clearColour;
    }

    void SetCulling(bool enabled) override {
        _culling = enabled;
        if (_culling)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }
    void IsCulling() override {
        return _culling;
    }

    void SetDepthTesting(bool enabled) override {
        _depthTesting = enabled;
        if (_depthTesting)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }
    void IsDepthTesting() override {
        return _depthTesting;
    }

    // set null to draw directly to screen
    void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) {
        _frameBuffer = fb;
        if (fb == nullptr)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        else
            fb->Bind();
    }
    std::shared_ptr<FrameBuffer> GetFrameBuffer() { 
        return _frameBuffer;
    }

    void SetProjection(const LA::mat4& proj) override {
        _projection = proj;
    }
    LA::mat4 GetProjection() override {
        return _projection;
    }

    void SetView(const LA::mat4& view) override {
        _view = view;
    }
    LA::mat4 GetView() override {
        return _view;
    }

    void Clear() override {
        if (_frameBuffer)
            _frameBuffer->Clear();
        else
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void SetClearColour(float r, float g, float b, float a) override {
        glClearColor(r, g, b, a);
    }

    void RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const LA::mat4& transform) {
        if (!shader || !Mesh) {
            std::cout << "ERROR (OpenGLRenderer): Attempting to render with null shader or mesh" << std::endl;
            return;
        }
        shader->Bind();
        shader->SetMat4("uView", &_view[0][0]);
        shader->SetMat4("uProjection", &_projection[0][0]);
        shader->SetMat4("uModel", &transform[0][0]);
        mesh->Draw();
    }
    
    // delete copy and assign operators
    // should always get instance from class::instance func
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // renderer is singleton
    static Renderer& Instance() {
        static Renderer instance;
        return instance;
    }

private:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;
};