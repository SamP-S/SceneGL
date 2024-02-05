#include "renderer/renderer.hpp"
#include "platform/opengl/opengl_renderer.hpp"

Renderer& Renderer::Instance() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            throw std::runtime_error("No Graphics API");
        case RendererAPI::API::OPENGL:
            return OpenGLRenderer::Instance();
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}
