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

#include "la_extended.h"
#include "renderer/renderer_api.hpp"

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

class Context {
protected:
    // opengl context settings
    DrawMode _drawMode = DrawMode::FILL;
    float _pointSize = 5.0f;
    float _lineWidth = 3.0f;
    bool _depthTesting = true;
    bool _culling = true;
    Winding _winding = Winding::CCW;
    LA::vec4 _clearColour = LA::vec4({0.35f, 0.35f, 0.5f, 1.0f});

public:
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

    Context() = default;
    ~Context() = default;

    // delete copy and assign operators should always get instance from class::instance func
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

    static std::shared_ptr<Context> Create();

};