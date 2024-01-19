#pragma once

#include "tai/tai.hpp"

class FrameBuffer : public Tai::Asset {
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
        virtual void SetClearColour(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0;

};
