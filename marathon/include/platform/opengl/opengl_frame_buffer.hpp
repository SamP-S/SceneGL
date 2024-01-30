#pragma once

#include "renderer/frame_buffer.hpp"
#include "platform/opengl/opengl.hpp"

class OpenGLFrameBuffer : public FrameBuffer {
private:
    uint32_t _fbo = 0;
    uint32_t _texColour = 0;
    uint32_t _texDepthStencil = 0;

    friend class Application;

public:
    OpenGLFrameBuffer(const std::string& name="Frame", int width=800, int height=600);
    ~OpenGLFrameBuffer();

    void Resize(int width, int height) override;
    void Bind() override;
    void Unbind() override;
    uint32_t GetColourAttachment() override;
    uint32_t GetDepthStencilAttachment() override;
    void Clear() override;
};
