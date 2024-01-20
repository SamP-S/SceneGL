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
        OpenGLShader(std::string name="Default Shader", std::shared_ptr<ShaderSource> vs=nullptr, std::shared_ptr<ShaderSource> fs=nullptr)
            : Shader(name, vs, fs) {
            Compile();
        }

        ~OpenGLShader() {
            glDeleteProgram(_programId);
        }
        
        // validity getter
        bool IsUsable() const override {
            return _validShader;
        }

        // OpenGL based calls
         void Compile() override {
            _validShader = false;
            if (vs == nullptr) {
                std::cout << "WARNING (Shader): Can't compile shader with null vertex shader source." << std::endl;
                return;
            }
            if (fs == nullptr) {
                std::cout << "WARNING (Shader): Can't compile shader with null fragment shader source." << std::endl;
                return;
            }
            if (vs->stage != ShaderStage::VERTEX || fs->stage != ShaderStage::FRAGMENT) {
                std::cout << "WARNING (Shader): Trying to compile non-matching stages." << std::endl;
                return;
            }

            try {
                // compile shader stages
                uint32_t vertex = vs->Compile();
                uint32_t fragment = fs->Compile();

                _programId = glCreateProgram();
                glAttachShader(_programId, vertex);
                glAttachShader(_programId, fragment);
                glBindFragDataLocation(_programId, 0, "oColour");
                glLinkProgram(_programId);
                
                // validate shader program correct
                int success;
                static char infoLog[1024];
                glGetProgramiv(_programId, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(_programId, 1024, NULL, infoLog);
                    throw infoLog;
                }

                // delete stages as opengl has copied to GPU
                glDeleteShader(vertex);
                glDeleteShader(fragment);

            } catch (const char* err) {
                std::cout << "Error: Shader::Compile - Complilation failed:" << std::endl;
                std::cout << err << std::endl;
                return;
            }
            _validShader = true;
        }

        void Bind() const override {
            glUseProgram(_programId);
        }

        void Unbind() const override {
            glUseProgram(0);
        }

        void SetBool(const std::string& name, bool value) const override {
            glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
        }

        void SetInt(const std::string& name, int value) const override {
            glUniform1i(glGetUniformLocation(_programId, name.c_str()), value);
        }

        void SetFloat(const std::string& name, float value) const override {
            glUniform1f(glGetUniformLocation(_programId, name.c_str()), value);
        }

        void SetVec2(const std::string& name, const LA::vec2& v) const override {
            glUniform2f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y);
        }

        void SetVec2(const std::string& name, float x, float y) const override {
            glUniform2f(glGetUniformLocation(_programId, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, const LA::vec3& v) const override {
            glUniform3f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y, v.z);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const override {
            glUniform3f(glGetUniformLocation(_programId, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, const LA::vec4& v) const override {
            glUniform4f(glGetUniformLocation(_programId, name.c_str()), v.x, v.y, v.z, v.w);
        }
        
        void SetVec4(const std::string& name, float x, float y, float z, float w) const override {
            glUniform4f(glGetUniformLocation(_programId, name.c_str()), x, y, z, w);
        }

        void SetMat4(const std::string& name, const LA::mat4& m) const override {
            glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, &(m)[0][0]);
        }

        void SetMat4(const std::string& name, float* mPtr) const override {
            glUniformMatrix4fv(glGetUniformLocation(_programId, name.c_str()), 1, GL_FALSE, mPtr);
        }

};
