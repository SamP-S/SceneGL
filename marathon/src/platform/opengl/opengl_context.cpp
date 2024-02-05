#include "platform/opengl/opengl_context.hpp"

OpenGLContext::OpenGLContext()  {
    SetPointSize(_pointSize);
    SetLineWidth(_lineWidth);
    SetWinding(_winding);
    SetClearColour(_clearColour);
    SetCulling(true);
    SetDepthTesting(true);
}

void OpenGLContext::SetDrawMode(DrawMode drawMode) {
    _drawMode = drawMode;
    if (_drawMode == DrawMode::POINTS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    else if (_drawMode == DrawMode::LINES)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

DrawMode OpenGLContext::GetDrawMode() {
    return _drawMode;
}

void OpenGLContext::SetPointSize(float size) {
    _pointSize = size;
    glPointSize(size);
}

float OpenGLContext::GetPointSize() {
    return _pointSize;
}

void OpenGLContext::SetLineWidth(float width) {
    _lineWidth = width;
    glLineWidth(_lineWidth);
}

float OpenGLContext::GetLineWidth() {
    return _lineWidth;
}

void OpenGLContext::SetWinding(Winding winding) {
    _winding = winding;
    if (winding == Winding::CW)
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
}

Winding OpenGLContext::GetWinding() {
    return _winding;
}

void OpenGLContext::SetClearColour(const LA::vec4& c) {
    _clearColour = c;
    glClearColor(c.r, c.g, c.b, c.a);
}

LA::vec4 OpenGLContext::GetClearColour() {
    return _clearColour;
}

void OpenGLContext::SetCulling(bool enabled) {
    _culling = enabled;
    if (_culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

bool OpenGLContext::IsCulling() {
    return _culling;
}

void OpenGLContext::SetDepthTesting(bool enabled) {
    _depthTesting = enabled;
    if (_depthTesting)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

bool OpenGLContext::IsDepthTesting() {
    return _depthTesting;
}
