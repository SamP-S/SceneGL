#include "renderer/mesh.hpp"
#include "platform/opengl/opengl_mesh.hpp"

std::shared_ptr<Mesh> Mesh::Create(std::string name) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:   
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<OpenGLMesh>(name);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}