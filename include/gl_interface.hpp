#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdexcept>
#include <cstdint>

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>

#include "la.hpp"
using namespace LA;

#define SHADER_INPUT_POSITION   "iPosition"
#define SHADER_INPUT_UV         "iUV"
#define SHADER_INPUT_NORMAL     "iNormal"
#define SHADER_INPUT_COLOUR     "iColour"
#define SHADER_OUTPUT_FRAG      "oColour"

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_UV       1
#define ATTRIB_LOC_NORMAL   2
#define ATTRIB_LOC_COLOUR   3

// add more types
#define TYPE_FLOAT  GL_FLOAT
#define TYPE_INT    GL_INT
#define TYPE_UINT   GL_UNSIGNED_INT

// Features add more
#define FEATURE_DEPTH   GL_DEPTH_TEST
#define FEATURE_CULL    GL_CULL_FACE

// Draw modes
#define DRAW_POINTS     GL_POINTS
#define DRAW_LINES      GL_LINES
#define DRAW_TRIANGLES  GL_TRIANGLES

// Culling Faces
#define FRONT_CW        GL_CW
#define FRONT_CCW       GL_CCW

class GL_Interface {

    public:

    // Buffer Objects ------------------------------------------------------


        static uint32_t GenVertexBufferObj(const std::vector<vec2>* data) {
            if (data->size() <= 0) {
                // std::cout << "Error: GL_Interface::GenVertexBufferObj - Empty data array" << std::endl;
                return -1;
            }
            uint32_t vbo = 0;
            glGenBuffers(1, &vbo);
            // std::cout << "glGenBuffers(" << 1 << ", " << vbo << ")" << std::endl;
            BindVertexBufferObj(vbo);
            glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(vec2), &data->front(), GL_STATIC_DRAW);
            // std::cout << "glBufferData(GL_ARRAY_BUFFER, " << data->size() * sizeof(vec3) << ", " << &data[0]<< ", GL_STATIC_DRAW)" << std::endl;
            ErrorCheck();
            return vbo;
        }

        static uint32_t GenVertexBufferObj(const std::vector<vec3>* data) {
            if (data->size() <= 0) {
                // std::cout << "Error: GL_Interface::GenVertexBufferObj - Empty data array" << std::endl;
                return -1;
            }
            uint32_t vbo = 0;
            glGenBuffers(1, &vbo);
            // std::cout << "glGenBuffers(" << 1 << ", " << vbo << ")" << std::endl;
            BindVertexBufferObj(vbo);
            glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(vec3), &data->front(), GL_STATIC_DRAW);
            // std::cout << "glBufferData(GL_ARRAY_BUFFER, " << data->size() * sizeof(vec3) << ", " << &data[0]<< ", GL_STATIC_DRAW)" << std::endl;
            ErrorCheck();
            return vbo;
        }

        static uint32_t GenVertexBufferObj(const std::vector<vec4>* data) {
            if (data->size() <= 0) {
                // std::cout << "Error: GL_Interface::GenVertexBufferObj - Empty data array" << std::endl;
                return -1;
            }
            uint32_t vbo = 0;
            glGenBuffers(1, &vbo);
            // std::cout << "glGenBuffers(" << 1 << ", " << vbo << ")" << std::endl;
            BindVertexBufferObj(vbo);
            glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(vec4), &data->front(), GL_STATIC_DRAW);
            // std::cout << "glBufferData(GL_ARRAY_BUFFER, " << data->size() * sizeof(vec3) << ", " << &data[0]<< ", GL_STATIC_DRAW)" << std::endl;
            ErrorCheck();
            return vbo;
        }

        static void BindVertexBufferObj(uint32_t vbo) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            // std::cout << "glBindBuffer(GL_ARRAY_BUFFER" << ", " << vbo << ")" << std::endl;
            ErrorCheck();
        }

        static uint32_t GenElementBufferObj(const std::vector<uint32_t>* indicies) {
            if (indicies->size() <= 0) {
                // std::cout << "Error: GL_Interface::GenElementBufferObj - Empty indicies array" << std::endl;
                return -1;
            }
            uint32_t ebo = 0;
            glGenBuffers(1, &ebo);
            // std::cout << "glGenBuffers(" << 1 << ", " << ebo << ")" << std::endl;
            BindElementBufferObj(ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies->size() * sizeof(uint32_t), &indicies->front(), GL_STATIC_DRAW);
            // std::cout << "glBufferData(GL_ELEMENT_ARRAY_BUFFER, " << indicies->size() * sizeof(uint32_t) << ", " << &indicies[0]<< ", GL_STATIC_DRAW)" << std::endl;
            ErrorCheck();
            return ebo;
        }

        static void BindElementBufferObj(uint32_t ebo) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            // std::cout << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER" << ", " << ebo << ")" << std::endl;
            ErrorCheck();
        }

        static void DelVertexBufferObj(uint32_t vbo) {
            glDeleteBuffers(1, &vbo);
            // std::cout << "glDeleteBuffers(1" << ", " << vbo << ")" << std::endl;
            ErrorCheck();
        }

    // Frame Buffer Objects --------------------------------------------------

        static uint32_t GenFrameBufferObj() {
            uint32_t fbo;
            glGenFramebuffers(1, &fbo);
            // std::cout << "glGenFramebuffers(1, " << fbo << ")" << std::endl;
            BindFrameBufferObj(fbo);
            ErrorCheck();
            return fbo;
        }

        static void BindFrameBufferObj(uint32_t fbo) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            // std::cout << "glBindFramebuffer(GL_FRAMEBUFFER, " << fbo << ")" << std::endl;
            ErrorCheck();
        }

        static void BindFrameBufferTexture2D(uint32_t fbo, uint32_t tex) {
            BindFrameBufferObj(fbo);
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                    GL_TEXTURE_2D, tex, 0);
            // std::cout << "glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, " << tex << ", " << 0 << ")" << std::endl;
            CheckFrameBuffer();
        }

        static void BindFrameBufferStencil2D(uint32_t fbo, uint32_t tex) {
            BindFrameBufferObj(fbo);
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                                    GL_TEXTURE_2D, tex, 0);
            // std::cout << "glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, " << tex << ", " << 0 << ")" << std::endl;
            CheckFrameBuffer();
        }

        static void CheckFrameBuffer() {
            uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (status != GL_FRAMEBUFFER_COMPLETE)
                throw "FrameBuffer has failed";
        }
        

    // Vertex Array Objects ---------------------------------------------------

        // swap later https://stackoverflow.com/questions/37972229/glvertexattribpointer-and-glvertexattribformat-whats-the-difference/37972230#37972230

        static uint32_t GenVertexArrayObject() {
            uint32_t vao = 0;
            glGenVertexArrays(1, &vao);
            // std::cout << "glGenVertexArrays(1, " << vao << ")" << std::endl;
            BindVertexArrayObject(vao);
            ErrorCheck();
            return vao;
        }

        static void BindVertexArrayObject(uint32_t vao) {
            glBindVertexArray(vao);
            // std::cout << "glBindVertexArray(" << vao << ")" << std::endl;
            ErrorCheck();
        }

        static void VertexAttribPtr(uint32_t attribLoc, int size, uint32_t type) {
            glVertexAttribPointer(attribLoc, size, type, GL_FALSE, 0, 0);
            // std::cout << "glVertexAttribPointer(" << attribLoc << ", " << size << ", " << type << ", GL_FALSE, 0, 0)" << std::endl;
            glEnableVertexAttribArray(attribLoc);
            // std::cout << "glEnableVertexAttribArray(" << attribLoc << ")" << std::endl;
            ErrorCheck();
        }

        static uint32_t GetAttribLoc(uint32_t program, const char* name) {
            return glGetAttribLocation(program, name);
        }

        static void EnableVertexAttribPtr(uint32_t program, const char* name) {
            uint32_t attribPtr = glGetAttribLocation(program, name);
            glEnableVertexAttribArray(attribPtr);
            ErrorCheck();
        }  

        static void DisableVertexAttribPtr(uint32_t program, const char* name) {
            uint32_t attribPtr = glGetAttribLocation(program, name);
            glDisableVertexAttribArray(attribPtr);
            ErrorCheck();
        }    
        
    // Shaders ------------------------------------------------------------------

        static uint32_t CreateShaderProgram(const char* vSource, const char* fSource) {
            uint32_t vertex = CreateVertexShader(vSource);
            uint32_t fragment = CreateFragmentShader(fSource);
            return LinkShaderProgram(vertex, fragment);
        }

        static bool UseShader(uint32_t shader) {
            glUseProgram(shader);
            // std::cout <<"glUseProgram(" << shader << ")" << std::endl;
            return ErrorCheck();
        }

    // Render Functions --------------------------------------------------------

        static void SetViewport(uint32_t width, uint32_t height) {
            glViewport(0, 0, width, height);
            // std::cout << "glViewport(0, 0, " << width << ", " << height << ")" << std::endl;
        }

        static void SetClearColour(float r, float g, float b, float a) {
            glClearColor(r, g, b, a);
            // std::cout << "glClearColor(" << r << ", "<< g << ", " << b << ", " << a << ")" << std::endl;
        }

        static void ClearColourDepth() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // std::cout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)" << std::endl;
        }

        static void EnableFeature(uint32_t feature) {
            glEnable(feature);
            // std::cout << "glEnable(" << feature << ")" << std::endl;
            ErrorCheck();
        }

        static void SetFrontFace(uint32_t direction) {
            glFrontFace(direction);
            ErrorCheck();
        }

        static void DisableFeature(uint32_t feature) {
            glDisable(feature);
            // std::cout << "glDisable(" << feature << ")" << std::endl;
            ErrorCheck();
        }

    // Draw Calls ----------------------------------------------------------------

        static void DrawArrays(uint32_t mode, uint32_t first, uint32_t count) {
            glDrawArrays(mode, first, count);
            // std::cout << "glDrawArrays(" << mode << ", " << first << ", " << count << ")" << std::endl;
            ErrorCheck();
        }

        static void DrawElements(uint32_t mode, uint32_t count, uint32_t type) {
            glDrawElements(mode, count, type, 0);
            // std::cout << "glDrawElements(" << mode << ", " << count << ", " << type << ", 0)" << std::endl;
            ErrorCheck();
        }

    // Textures -----------------------------------------------------------------

        // create definitions for texture parameters

        static uint32_t GenTexture2D(uint32_t width, uint32_t height) {
            uint32_t tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            ErrorCheck();
            return tex;
        }

        static uint32_t GenStencil2D(uint32_t width, uint32_t height) {
            uint32_t tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
                            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
            ErrorCheck();
            return tex;
        }

        static void BindTexture(uint32_t tex) {
            glBindTexture(GL_TEXTURE_2D, tex);
            // std::cout << "glBindTexture(GL_TEXTURE_2D, " << tex << ")" << std::endl;
            ErrorCheck();
        }

    // Validation ------------------------------------------------------------

        static bool ErrorCheck() {
            uint32_t error = glGetError();

            if (error == GL_NO_ERROR) {
                // // std::cout << "OK: all good" << std::endl;
                return true;
            } else {
                // std::cout << "ERROR: ";
            }

            if (error == GL_INVALID_OPERATION) {
                // std::cout << "INVALID OPERATION" << std::endl;
            } else if (error == GL_INVALID_VALUE) {
                // std::cout << "INVALID VALUE" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                // std::cout << "NO MEMORY" << std::endl;
            } else if (error == GL_STACK_OVERFLOW) {
                // std::cout << "NO OVERFLOW" << std::endl;
            } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
                // std::cout << "Invalid Framebuffer" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                // std::cout << "Out of Memory" << std::endl;
            }
            return false;
        }

    private:
        
        // Shader ---------------------------------------------------------------
        
        static void CheckShaderCompile(uint32_t shader) {
            int success;
            static char infoLog[1024];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                throw infoLog;
            }
        }

        static void CheckProgramCompile(uint32_t program) {
            int success;
            static char infoLog[1024];
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(program, 1024, NULL, infoLog);
                throw infoLog;
            }
        }

        static uint32_t CreateVertexShader(const char* source) {
            return CreateShader(source, GL_VERTEX_SHADER);
        }

        static uint32_t CreateFragmentShader(const char* source) {
            return CreateShader(source, GL_FRAGMENT_SHADER);
        }

        static uint32_t CreateShader(const char* source, uint32_t type) {
            uint32_t shader = glCreateShader(type);
            // std::cout << "glCreateShader(" << type << ")" << std::endl;
            glShaderSource(shader, 1, &source, NULL);
            // std::cout << "glShaderSource(" << shader << ", 1, " << &source << ", NULL)" << std::endl;
            glCompileShader(shader);
            // std::cout << "glCompileShader(" << shader << ")" << std::endl;
            CheckShaderCompile(shader);
            return shader;
        }

        static uint32_t LinkShaderProgram(uint32_t vertex, uint32_t fragment) {
            uint32_t program = glCreateProgram();
            // std::cout << "glCreateProgram()" << std::endl;
            glAttachShader(program, vertex);
            // std::cout << "glAttachShader(" << program << ", " << vertex << ")" << std::endl;
            glAttachShader(program, fragment);
            // std::cout << "glAttachShader(" << program << ", " << fragment << ")" << std::endl;
            // glBindFragDataLocation(program, 0, SHADER_OUTPUT_FRAG);
            // // std::cout << "glBindFragDataLocation(" << program << ", " << 0 << ", SHADER_OUTPUT_FRAG)" << std::endl;
            glBindFragDataLocation(program, 0, SHADER_OUTPUT_FRAG);
            glLinkProgram(program);
            // std::cout << "glLinkProgram(" << program << ")" << std::endl;
            CheckProgramCompile(program);
            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            return program;
        }
};
