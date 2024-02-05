#pragma once

#include "ecs/asset.hpp"
#include "renderer/renderer_api.hpp"

class FrameBuffer : public Asset {
protected:
    int _width = 0;
    int _height = 0;
    
    FrameBuffer(const std::string& name="FrameBuffer", int width=800, int height=600)
        : Asset(name), _width(width), _height(height) {}

public:
    
    // handle resolution
    int GetWidth() { return _width; }
    int GetHeight() { return _height; }
    
    // platform specific calls
    virtual void Resize(int width, int height) = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual uint32_t GetColourAttachment() = 0;
    virtual uint32_t GetDepthStencilAttachment() = 0;
    virtual void Clear() = 0;

    static std::shared_ptr<FrameBuffer> Create(const std::string& name="Frame", int width=800, int height=600);
        
};
