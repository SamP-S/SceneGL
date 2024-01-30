#include "platform/opengl/opengl_material.hpp"   

OpenGLMaterial::OpenGLMaterial(std::string name)
    : Material(name) {}

bool OpenGLMaterial::IsUsable() const {
    return shader != nullptr && shader->IsUsable();
}

void OpenGLMaterial::Bind() {
    if (!IsUsable()) {
        std::cout << "WARNING (OpenGL): Shader is not valid, can't bind material." << std::endl;
        return;
    }
    shader->Bind();
    for (auto& [key, value] : properties) {
        std::visit([this, &key](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, bool>) {
                shader->SetBool(key, arg);
            } else if constexpr (std::is_same_v<T, int>) {
                shader->SetInt(key, arg);
            } else if constexpr (std::is_same_v<T, float>) {
                shader->SetFloat(key, arg);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                shader->SetUint(key, arg);
            } else if constexpr (std::is_same_v<T, LA::vec2>) {
                shader->SetVec2(key, arg);
            } else if constexpr (std::is_same_v<T, LA::vec3>) {
                shader->SetVec3(key, arg);
            } else if constexpr (std::is_same_v<T, LA::vec4>) {
                shader->SetVec4(key, arg);
            } else if constexpr (std::is_same_v<T, LA::mat2>) {
                shader->SetMat2(key, arg);
            } else if constexpr (std::is_same_v<T, LA::mat3>) {
                shader->SetMat3(key, arg);
            } else if constexpr (std::is_same_v<T, LA::mat4>) {
                shader->SetMat4(key, arg);
            }
        }, value);
    }
}

void OpenGLMaterial::Unbind() {
    shader->Unbind();
}