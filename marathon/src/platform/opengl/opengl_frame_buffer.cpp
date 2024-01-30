#include "platform/opengl/opengl_frame_buffer.hpp"

/// TODO: Use glTexImage2DMultisample with a sample count for antialiasing affects
/// implement more openly to allow multiple colour attachments


OpenGLFrameBuffer::OpenGLFrameBuffer(const std::string& name, int width, int height)
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

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteTextures(1, &_texColour);
    glDeleteTextures(1, &_texDepthStencil);
    glDeleteFramebuffers(1, &_fbo);
}

void OpenGLFrameBuffer::Resize(int width, int height) {
    _width = width;
    _height = height;

    // Resize color texture
    glBindTexture(GL_TEXTURE_2D, _texColour);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Resize depth and stencil texture
    glBindTexture(GL_TEXTURE_2D, _texDepthStencil);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
}

void OpenGLFrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
}

void OpenGLFrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t OpenGLFrameBuffer::GetColourAttachment() {
    return _texColour;
}

uint32_t OpenGLFrameBuffer::GetDepthStencilAttachment() {
    return _texDepthStencil;
}

void OpenGLFrameBuffer::Clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
