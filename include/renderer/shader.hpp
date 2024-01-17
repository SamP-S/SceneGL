#pragma once

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ecs/resource.hpp"
#include "ecs/resource_manager.hpp"
#include "renderer/shader_stage.hpp"
#include "renderer/gl_interface.hpp"

class Shader : public Resource {
    private:
        ShaderStage* vs = new ShaderStage();
        ShaderStage* fs = new ShaderStage();
        bool _validShader = false;
        uint32_t _id = -1;

        bool Compile() {
            if (vs == nullptr || fs == nullptr) {
                std::cout << "WARNING (Shader): Can't compile shader with null stage." << std::endl;
                return false;
            }

            if (vs->GetStage() != SHADER_VERTEX || fs->GetStage() != SHADER_FRAGMENT) {
                std::cout << "WARNING (Shader): Trying to compile non matching stages." << std::endl;
                return false;
            }

            // shader Program
            try {
                _id = GL_Interface::CreateShaderProgram(fs->GetSource().c_str(), vs->GetSource().c_str());
            } catch (const char* err) {
                std::cout << "Error: Shader::Compile - Complilation failed";
                std::cout << err << std::endl;
                return false;
            }
            return true;
        }

    public:
        Shader(std::string name="Default Shader", ShaderStage* vs=nullptr, ShaderStage* fs=nullptr)
            : Resource(name) {
            _validShader = Compile();
        }

        // JSON
        std::string Type() {
            return "Shader";
        }
        void FromJson(json j) {
            return;
        }
        json ToJson() {
            json j;
            return j;
        }
        std::string ToString() {
            return "Shader";
        }
        
        // validity getter
        bool IsValid() {
            return _validShader;
        }

        // OpenGL based calls
        bool Use() {
            return GL_Interface::UseShader(_id);
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

extern ResourceManager<Shader> resourceShaders;