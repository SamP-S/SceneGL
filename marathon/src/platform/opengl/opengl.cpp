#include "platform/opengl/opengl.hpp"

bool OpenGLErrorCheck() {
    uint32_t error = glGetError();
    if (error == GL_NO_ERROR) {
        return true;
    }
    
    std::cout << "ERROR (OpenGL): ";
    switch (error) {
        case GL_INVALID_OPERATION:
            std::cout << "operation is not allowed in the current state" << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "numeric argument is out of range" << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "stack overflow" << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "unacceptable value for an enum arg" << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "framebuffer invalid or not complete" << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "out of GPU memory" << std::endl;
            break;
        case GL_INVALID_ENUM:
            std::cout << "unacceptable value for an enum arg" << std::endl;
            break;
        default:
            std::cout << "unknown error" << std::endl;
            break;
    }
    return false;
}