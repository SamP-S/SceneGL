#include "renderer/shader.hpp"
#include "platform/opengl/opengl_shader.hpp"

std::shared_ptr<Shader> Shader::Create(const std::string& name, std::shared_ptr<ShaderSource> vs, std::shared_ptr<ShaderSource> fs) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<OpenGLShader>(name, vs, fs);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}