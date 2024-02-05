#include "renderer/shader_source.hpp"
#include "platform/opengl/opengl_shader_source.hpp"

std::shared_ptr<ShaderSource> ShaderSource::Create(const std::string& name, const std::string& source, const ShaderStage& stage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<OpenGLShaderSource>(name, source, stage);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}
