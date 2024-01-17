#pragma once

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gl_interface.hpp"
#include "resource.hpp"
#include "resource_manager.hpp"

#define SHADER_INVALID  0
#define SHADER_VERTEX   1
#define SHADER_FRAGMENT 2
#define SHADER_COMPUTE  3
#define SHADER_GEOMETRY 4
#define SHADER_TESSELLATION_CONTROL     5
#define SHADER_TESSELLATION_EVALUATION  6

class Shader : public Resource {

    private:
        bool _validShader = false;
        int _shaderType = SHADER_INVALID;

        bool Compile() {
            const char* vShaderCode = vertexCode.c_str();
            const char * fShaderCode = fragmentCode.c_str();
            
            // shader Program
            try {
                this->id = GL_Interface::CreateShaderProgram(vShaderCode, fShaderCode);
            } catch (const char* err) {
                std::cout << "Error: Shader::Compile - Complilation failed";
                std::cout << err << std::endl;
                return false;
            }
            return true;
        }


    public:
        unsigned int id;
        std::string vertexCode = "";
        std::string fragmentCode = "";

        Shader(std::string name, std::string vs, std::string fs)
            : Resource(name) {
            if (vs == "" || fs == "") {
                throw "Shader Source Invalid";
            }
            this->vertexCode = vs;
            this->fragmentCode = fs;
            this->_validShader = Compile();
        }

        Shader(std::string name, const char* vFilePath, const char* fFilePath)
            : Shader(name, GetSourceFromFile(vFilePath), GetSourceFromFile(fFilePath)) {
        }

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


        bool Use() {
            return GL_Interface::UseShader(this->id);
        }

        bool isValid() {
            return _validShader;
        }

        void SetBool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(id, name.c_str()), value);
        }

        void SetInt(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(id, name.c_str()), value);
        }

        void SetFloat(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(id, name.c_str()), value);
        }

        void SetVec2(const std::string& name, vec2 v) const {
            glUniform2f(glGetUniformLocation(id, name.c_str()), v.x, v.y);
        }

        void SetVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, vec3 v) const {
            glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, vec4 v) const {
            glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
        }
        
        void SetVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
        }

        void SetMat4(const std::string& name, float* mPtr) const {
            glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, mPtr);
        }

};

extern ResourceManager<Shader> resourceShaders;