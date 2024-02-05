#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "la_extended.h"

#include "ecs/asset.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/shader_source.hpp"

class Shader : public Asset {
public:
    std::shared_ptr<ShaderSource> vs = nullptr;
    std::shared_ptr<ShaderSource> fs = nullptr;

    Shader(const std::string& name, std::shared_ptr<ShaderSource> vs=nullptr, std::shared_ptr<ShaderSource> fs=nullptr)
        : Asset(name), vs(vs), fs(fs) {}
    
    // check shader validity
    virtual bool IsUsable() const = 0;

    // Renderer Impl calls
    virtual void Compile() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetBool(const std::string& name, bool value) const = 0;
    virtual void SetInt(const std::string& name, int value) const = 0;
    virtual void SetUint(const std::string& name, uint32_t value) const = 0;
    virtual void SetFloat(const std::string& name, float value) const = 0;
    virtual void SetVec2(const std::string& name, const LA::vec2& v) const = 0;
    virtual void SetVec2(const std::string& name, float x, float y) const = 0;
    virtual void SetVec3(const std::string& name, const LA::vec3& v) const = 0;
    virtual void SetVec3(const std::string& name, float x, float y, float z) const = 0;
    virtual void SetVec4(const std::string& name, const LA::vec4& v) const = 0;
    virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const = 0;
    virtual void SetMat2(const std::string& name, const LA::mat2& m) const = 0;
    virtual void SetMat2(const std::string& name, float* mPtr) const = 0;
    virtual void SetMat3(const std::string& name, const LA::mat3& m) const = 0;
    virtual void SetMat3(const std::string& name, float* mPtr) const = 0;
    virtual void SetMat4(const std::string& name, const LA::mat4& m) const = 0;
    virtual void SetMat4(const std::string& name, float* mPtr) const = 0;

    static std::shared_ptr<Shader> Create(const std::string& name, std::shared_ptr<ShaderSource> vs, std::shared_ptr<ShaderSource> fs);

};
