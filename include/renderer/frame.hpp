#pragma once

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>


#include "gl_interface.hpp"

class Frame {

    public:
        Frame(int width, int height) {
            _frameBufferId = GL_Interface::GenFrameBufferObj();
            _frameTexColour = GL_Interface::GenFBTexture2D(width, height);
            GL_Interface::BindFrameBufferTexture2D(_frameBufferId, _frameTexColour);
            _frameTexDepthStencil = GL_Interface::GenFBStencil2D(width, height);
            GL_Interface::BindFrameBufferStencil2D(_frameBufferId, _frameTexDepthStencil);
        }
        ~Frame() {
            /// TODO: Delete fbo and textures for safe close
        }

        // Bind this frame buffer
        void Bind() {
            GL_Interface::BindFrameBufferObj(_frameBufferId);
        }

        // Unbind frame buffer to default (0)
        void Unbind() {
            GL_Interface::BindFrameBufferObj(0);
        }

        uint32_t GetTexColour() {
            return _frameTexColour;
        }

        uint32_t GetTexDepthStencil() {
            return _frameTexDepthStencil;
        }

        // void AttachTexture(uint32_t attachment, uint32_t textureId);
        // void AttachRenderbuffer(uint32_t attachment, uint32_t renderbufferId);

        void SetClearColour(float r, float g, float b, float a) {
            _clearR = r;
            _clearG = g;
            _clearB = b;
            _clearA = a;
            GL_Interface::SetClearColour(r, g, b, a);
        }

        // Clear colour and depth textures in frame buffer
        void Clear() {
            GL_Interface::ClearColourDepth();
        }


    private:
        uint32_t _frameBufferId, _frameTexColour, _frameTexDepthStencil = 0;
        float _clearR, _clearG, _clearB = 0.0f;
        float _clearA = 1.0f;
};
