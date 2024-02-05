#pragma once

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
    void Boot() override;
    void Shutdown() override;

    // set null to draw directly to window
    void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb);
    std::shared_ptr<FrameBuffer> GetFrameBuffer();
    void SetProjection(const LA::mat4& proj) override;
    LA::mat4 GetProjection() override;
    void SetView(const LA::mat4& view) override;
    LA::mat4 GetView() override;

    void Clear() override;
    void RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const LA::mat4& transform) override;
    static Renderer& Instance();

protected:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() = default;
};