#pragma once

#include "platform/opengl/opengl.hpp"
#include "renderer/frame_buffer.hpp"

/// TODO: Use glTexImage2DMultisample with a sample count for antialiasing affects
/// implement more openly to allow multiple colour attachments

class OpenGLFrameBuffer : public FrameBuffer {
    private:
        uint32_t _fbo = 0;
        uint32_t _texColour = 0;
        uint32_t _texDepthStencil = 0;

        friend class Application;

    public:
        OpenGLFrameBuffer(const std::string& name="Frame", int width=800, int height=600)
            : FrameBuffer(name, width, height) {
            // create fbo
            glGenFramebuffers(1, &_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

            // create colour texture for attachment
            glGenTextures(1, &_texColour);
            glBindTexture(GL_TEXTURE_2D, _texColour);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            
            // Note: Must keep otherwise Min/Mag default to using mipmaps that aren't generated.
            // Causes ImGui to draw the texture as black.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // bind to colour attachment
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texColour, 0);

            // create depth and stencil 
            glGenTextures(1, &_texDepthStencil);
            glBindTexture(GL_TEXTURE_2D, _texDepthStencil);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

            // bind to depth and stencil
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _texDepthStencil, 0);
            
            // unbind for safety
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        ~OpenGLFrameBuffer() {
            glDeleteTextures(1, &_texColour);
            glDeleteTextures(1, &_texDepthStencil);
            glDeleteFramebuffers(1, &_fbo);
        }

        // resize frame buffer attachments
        void Resize(int width, int height) override {
            _width = width;
            _height = height;

            // Resize color texture
            glBindTexture(GL_TEXTURE_2D, _texColour);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            // Resize depth and stencil texture
            glBindTexture(GL_TEXTURE_2D, _texDepthStencil);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        }

        // Bind this frame buffer
        void Bind() override {
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            glViewport(0, 0, _width, _height);
        }

        // Unbind frame buffer to default (0)
        void Unbind() override {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        uint32_t GetColourAttachment() override {
            return _texColour;
        }

        // binds fbo then sets clear colour
        void SetClearColour(float r, float g, float b, float a) override {
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            glClearColor(r, g, b, a);
        }

        // binds fbo then clears colour, depth and stencil texture attachments
        void Clear() override {
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
};
