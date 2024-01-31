#pragma once

#include "ecs/asset.hpp"

class FrameBuffer : public Asset {
    protected:
        int _width = 0;
        int _height = 0;

    public:
        FrameBuffer(const std::string& name, int width, int height)
            : Asset(name), _width(width), _height(height) {}
        
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

};
