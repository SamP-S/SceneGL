#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "GL_Interface.hpp"

/*
    IMPLEMENT
    glGetShaderiv
    glGetShaderInfoLog
    glGetAttribLocation
*/

class Shader {

    private:
        bool _validShader = false;

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

         static std::string GetSourceFromFile(const char* filePath) {
            std::string code = "";
            std::ifstream shaderFile;
            // ensure ifstream objects can throw exceptions:
            shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            try {
                // open files
                shaderFile.open(filePath);
                std::stringstream shaderStream;
                // read file's buffer contents into streams
                shaderStream << shaderFile.rdbuf();
                // close file handlers
                shaderFile.close();
                // convert stream into string
                code = shaderStream.str();
                // std::cout << code << std::endl;
            }
            catch (std::ifstream::failure& e) {
                std::cout << e.what() << std::endl;
                std::cout << "Error::Shader - FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            return code;
        }

    public:
        unsigned int id;
        std::string vertexCode = "";
        std::string fragmentCode = "";

        Shader(std::string vs, std::string fs) {
            if (vs == "" || fs == "") {
                throw "Shader Source Invalid";
            }
            this->vertexCode = vs;
            this->fragmentCode = fs;
            this->_validShader = Compile();
        }

        Shader(const char* vFilePath, const char* fFilePath) :
            Shader(GetSourceFromFile(vFilePath), GetSourceFromFile(fFilePath)) {
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

        void SetVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
        }

        void SetVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
        }

        void SetVec4(const std::string& name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
        }

};