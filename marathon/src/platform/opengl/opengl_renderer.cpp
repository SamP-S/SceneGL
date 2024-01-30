#include "platform/opengl/opengl_renderer.hpp"

void OpenGLRenderer::Boot() {
    std::cout << "DEBUG (OpenGLRenderer): Boot." << std::endl;   
    SetPointSize(_pointSize);
    SetLineWidth(_lineWidth);
    SetWinding(_winding);
    SetClearColour(_clearColour);
    SetCulling(true);
    SetDepthTesting(true);
}

void OpenGLRenderer::Shutdown() {
    std::cout << "DEBUG (OpenGLRenderer): Shutdown." << std::endl;
}

void OpenGLRenderer::SetDrawMode(DrawMode drawMode) {
    _drawMode = drawMode;
    if (_drawMode == DrawMode::POINTS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    else if (_drawMode == DrawMode::LINES)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

DrawMode OpenGLRenderer::GetDrawMode() {
    return _drawMode;
}

void OpenGLRenderer::SetPointSize(float size) {
    _pointSize = size;
    glPointSize(size);
}

float OpenGLRenderer::GetPointSize() {
    return _pointSize;
}

void OpenGLRenderer::SetLineWidth(float width) {
    _lineWidth = width;
    glLineWidth(_lineWidth);
}

float OpenGLRenderer::GetLineWidth() {
    return _lineWidth;
}

void OpenGLRenderer::SetWinding(Winding winding) {
    _winding = winding;
    if (winding == Winding::CW)
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
}

Winding OpenGLRenderer::GetWinding() {
    return _winding;
}

void OpenGLRenderer::SetClearColour(const LA::vec4& c) {
    _clearColour = c;
    glClearColor(c.r, c.g, c.b, c.a);
}

LA::vec4 OpenGLRenderer::GetClearColour() {
    return _clearColour;
}

void OpenGLRenderer::SetCulling(bool enabled) {
    _culling = enabled;
    if (_culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

bool OpenGLRenderer::IsCulling() {
    return _culling;
}

void OpenGLRenderer::SetDepthTesting(bool enabled) {
    _depthTesting = enabled;
    if (_depthTesting)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

bool OpenGLRenderer::IsDepthTesting() {
    return _depthTesting;
}

void OpenGLRenderer::SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) {
    _frameBuffer = fb;
    if (fb == nullptr)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else
        fb->Bind();
}

std::shared_ptr<FrameBuffer> OpenGLRenderer::GetFrameBuffer() {
    return _frameBuffer;
}

void OpenGLRenderer::SetProjection(const LA::mat4& proj) {
    _projection = proj;
}

LA::mat4 OpenGLRenderer::GetProjection() {
    return _projection;
}

void OpenGLRenderer::SetView(const LA::mat4& view) {
    _view = view;
}

LA::mat4 OpenGLRenderer::GetView() {
    return _view;
}

void OpenGLRenderer::Clear() {
    if (_frameBuffer)
        _frameBuffer->Clear();
    else
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::RenderMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, const LA::mat4& transform) {
    if (!shader || !mesh) {
        std::cout << "ERROR (OpenGLRenderer): Attempting to render with null shader or mesh" << std::endl;
        return;
    }
    shader->Bind();
    shader->SetMat4("uView", _view);
    shader->SetMat4("uProjection", _projection);
    shader->SetMat4("uModel", transform);
    mesh->Draw();
}

Renderer& OpenGLRenderer::Instance() {
    static Renderer* instance;
    if (!instance)
        instance = new OpenGLRenderer();
    return *instance;
}
