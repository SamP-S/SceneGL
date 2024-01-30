#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "platform/opengl/opengl.hpp"
#include "renderer/shader.hpp"

//// TODO:
// unfix binding fragment data output location to "oColour"

class OpenGLShader : public Shader {
    private:
        bool _validShader = false;
        uint32_t _programId = 0;

    public:
        OpenGLShader(std::string name="Default Shader", std::shared_ptr<ShaderSource> vs=nullptr, std::shared_ptr<ShaderSource> fs=nullptr);
        ~OpenGLShader();
        
        bool IsUsable() const override;

        void Compile() override;
        void Bind() const override;
        void Unbind() const override;

        void SetBool(const std::string& name, bool value) const override;
        void SetInt(const std::string& name, int value) const override;
        void SetUint(const std::string& name, uint32_t value) const override;
        void SetFloat(const std::string& name, float value) const override;
        void SetVec2(const std::string& name, const LA::vec2& v) const override;
        void SetVec2(const std::string& name, float x, float y) const override;
        void SetVec3(const std::string& name, const LA::vec3& v) const override;
        void SetVec3(const std::string& name, float x, float y, float z) const override;
        void SetVec4(const std::string& name, const LA::vec4& v) const override;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const override;
        void SetMat2(const std::string& name, const LA::mat2& m) const override;
        void SetMat2(const std::string& name, float* mPtr) const override;
        void SetMat3(const std::string& name, const LA::mat3& m) const override;
        void SetMat3(const std::string& name, float* mPtr) const override;
        void SetMat4(const std::string& name, const LA::mat4& m) const override;
        void SetMat4(const std::string& name, float* mPtr) const override;
};
