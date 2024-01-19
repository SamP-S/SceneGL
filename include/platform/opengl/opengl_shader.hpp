#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "platform/opengl/opengl.hpp"
#include "renderer/shader.hpp"

class OpenGLShader : public Shader {
    private:
        bool _isValid = false;
        uint32_t _id = 0;

    public:
        OpenGLShader(std::string name="Default Shader", std::shared_ptr<ShaderStage> vs=nullptr, std::shared_ptr<ShaderStage> fs=nullptr)
            : Shader(name, vs, fs) {
            Compile();
        }
        
        // validity getter
        bool IsUsable() const {
            return _validShader;
        }

        // OpenGL based calls
         void Compile() {
            _validShader = false;
            if (vs == nullptr) {
                std::cout << "WARNING (Shader): Can't compile shader with null vertex shader source." << std::endl;
                return;
            }
            if (fs == nullptr) {
                std::cout << "WARNING (Shader): Can't compile shader with null fragment shader source." << std::endl;
                return;
            }
            if (vs.stage != ShaderStage::VERTEX || fs.stage != ShaderStage::FRAGMENT) {
                std::cout << "WARNING (Shader): Trying to compile non-matching stages." << std::endl;
                return;
            }

            try {
                // compile shader stages
                uint32_t vertex = vs.Compile();
                uint32_t fragment = fs.Compile();

                _id = glCreateProgram();
                glAttachShader(_id, vertex);
                glAttachShader(_id, fragment);
                glBindFragDataLocation(_id, 0, SHADER_OUTPUT_FRAG);
                glLinkProgram(_id);
                
                // validate shader program correct
                int success;
                static char infoLog[1024];
                glGetProgramiv(program, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(program, 1024, NULL, infoLog);
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

        void Bind() const {
            glUseProgram(shader);
        }

        void Unbind() const {
            glUseProgram(0);
        }

        void SetBool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
        }

        void SetInt(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
        }

        void SetFloat(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
        }

        void SetVec2(const std::string& name, vec2 v) const {
            glUniform2f(glGetUniformLocation(_id, name.c_str()), v.x, v.y);
        }

        void SetVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, vec3 v) const {
            glUniform3f(glGetUniformLocation(_id, name.c_str()), v.x, v.y, v.z);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, vec4 v) const {
            glUniform4f(glGetUniformLocation(_id, name.c_str()), v.x, v.y, v.z, v.w);
        }
        
        void SetVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
        }

        void SetMat4(const std::string& name, float* mPtr) const {
            glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, mPtr);
        }

};
