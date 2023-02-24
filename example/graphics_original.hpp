#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>

// includes
#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#include "window.hpp"
#include "frame_timer.hpp"
#include "shader.hpp"
#include "texture.hpp"

enum Comparison {
    NEVER = 0,
    ALWAYS = 1,
    LESS = 2,
    LEQUAL = 3,
    EQUAL = 4,
    NOT_EQUAL = 5,
    GEQUAL = 6,
    GREATER = 7
};

class GraphicsEngine {
    private:
        unsigned int frameNum = 0;

    public:
        void* context;
        WindowManager* window;
        frame_timer ft;
        Shader shader;
        GLuint vbo, vao, fbo, texColour, texDepthStencil;
        int width, height;
        Texture* texBuffer[4] = { new Texture(), new Texture(), new Texture(), new Texture() };
        

        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            InitGL();
            ft = frame_timer();
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
        }

        bool AttachWindow(WindowManager* window) {
            if (window == NULL) {
                return false;
            }
            this->window = window;
            return true;
        }

        bool DetachWindow() {
            this->window = NULL;
            return true;
        }

        void InitGL() {
            // Initialise GLEW
            glewExperimental = GL_TRUE;
            glewInit();

            float vertices[] = {
                -1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f
            };
            
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
                std::cout << "COOL: FBO hasn't yabber dabber died yet" << std::endl;

            glGenTextures(1, &texColour);
            
            glBindTexture(GL_TEXTURE_2D, texColour);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                    GL_TEXTURE_2D, texColour, 0);

            glGenTextures(1, &texDepthStencil);
            glBindTexture(GL_TEXTURE_2D, texDepthStencil);
            glTexImage2D(   GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, 
                            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
            );

            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                                    GL_TEXTURE_2D, texDepthStencil, 0);

            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(status != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
                
            glBindFramebuffer(GL_FRAMEBUFFER, 0); 


            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            shader = Shader("shaders\\base.vs", "shaders\\cool.fs");
            glBindFragDataLocation(shader.ID, 0, "oColour");
            shader.Use();

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            GLint posAttrib = glGetAttribLocation(shader.ID, "iPosition");
            glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(posAttrib);
        }
        
        // function to reset our viewport after a window resize
        int SetViewport(int _width, int _height) {
            this->width = _width;
            this->height = _height;

            /* Protect against a divide by zero */
            if (height == 0 ) {
                height = 1;
            }
            float ratio = width / height;

            return 1;
        }

        static GLenum GetGLTarget(Tex_Target _target) {
            switch (_target) {
                case Tex_Target::TEXTURE_1D:
                    return GL_TEXTURE_1D;
                case Tex_Target::TEXTURE_2D:
                    return GL_TEXTURE_2D;
                case Tex_Target::TEXTURE_2D_MULTISAMPLE:
                    return GL_TEXTURE_2D_MULTISAMPLE;
                case Tex_Target::TEXTURE_3D:
                    return GL_TEXTURE_3D;
                case Tex_Target::TEXTURE_BUFFER:
                    return GL_TEXTURE_BUFFER;
                case Tex_Target::TEXTURE_CUBE_MAP:
                    return GL_TEXTURE_CUBE_MAP;
                case Tex_Target::TEXTURE_RECTANGLE:
                    return GL_TEXTURE_RECTANGLE;
            }
        }

        static GLenum GetGLWrapAxis(Tex_Wrap_Axis _axis) {
            switch (_axis) {
                case Tex_Wrap_Axis::TEXTURE_WRAP_S:
                    return GL_TEXTURE_WRAP_S;
                case Tex_Wrap_Axis::TEXTURE_WRAP_T:
                    return GL_TEXTURE_WRAP_T;
                case Tex_Wrap_Axis::TEXTURE_WRAP_R:
                    return GL_TEXTURE_WRAP_R;
            }
        }

        static GLint GetGLWrapping(Tex_Wrapping _wrap) {
            switch (_wrap) {
                case Tex_Wrapping::REPEAT:
                    return GL_REPEAT;
                case Tex_Wrapping::CLAMP_TO_BORDER:
                    return GL_CLAMP_TO_BORDER;
                case Tex_Wrapping::CLAMP_TO_EDGE:
                    return GL_CLAMP_TO_EDGE;
                case Tex_Wrapping::MIRRORED_REPEAT:
                    return GL_MIRRORED_REPEAT;
            }
        }

        static GLenum GetGLFilterLevel(Tex_Filter_Level _level) {
            switch (_level) {
                case Tex_Filter_Level::TEXTURE_MIN_FILTER:
                    return GL_TEXTURE_MIN_FILTER;
                case Tex_Filter_Level::TEXTURE_MAG_FILTER:
                    return GL_TEXTURE_MAG_FILTER;
            }
        }

        static GLint GetGLFiltering(Tex_Filtering _filter) {
            switch (_filter) {
                case Tex_Filtering::LINEAR:
                    return GL_LINEAR;
                case Tex_Filtering::LINEAR_MIPMAP_LINEAR:
                    return GL_LINEAR_MIPMAP_LINEAR;
                case Tex_Filtering::LINEAR_MIPMAP_NEAREST:
                    return GL_LINEAR_MIPMAP_NEAREST;
                case Tex_Filtering::NEAREST:
                    return GL_NEAREST;
                case Tex_Filtering::NEAREST_MIPMAP_LINEAR:
                    return GL_NEAREST_MIPMAP_LINEAR;
                case Tex_Filtering::NEAREST_MIPMAP_NEAREST:
                    return GL_NEAREST_MIPMAP_NEAREST;
            }
        }

        static GLenum SetTexFiltering(Tex_Target _target, Tex_Filter_Level _level, Tex_Filtering _filter) {
            glTexParameteri(GetGLTarget(_target), GetGLFilterLevel(_level), GetGLFiltering(_filter));
        }

        static void SetTexWrapping(Tex_Target _target, Tex_Wrap_Axis _axis, Tex_Wrapping _wrap) {
            glTexParameteri(GetGLTarget(_target), GetGLWrapAxis(_axis), GetGLWrapping(_wrap));
        }

        static void BindTexture(Texture _tex) {
            glBindTexture(GetGLTarget(_tex.GetTarget()), _tex.GetID());
        }

        void Render() {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
            /* Clear The Screen And The Depth Buffer */
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glBindVertexArray(vao);
            glDisable(GL_DEPTH_TEST);
            shader.Use();

            shader.SetVec3("iResolution", window->width, window->height, 1.0f);
            shader.SetFloat("iTime", ft.GetTotalElapsed());
            shader.SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader.SetInt("iFrame", frameNum);

            glActiveTexture(GL_TEXTURE0 + texBuffer[0]->GetID());
            shader.SetInt("iChannel0", texBuffer[0]->GetID());
            glBindTexture(GL_TEXTURE_2D, texBuffer[0]->GetID());

            glActiveTexture(GL_TEXTURE0 + texBuffer[1]->GetID());
            shader.SetInt("iChannel1", texBuffer[1]->GetID());
            glBindTexture(GL_TEXTURE_2D, texBuffer[1]->GetID());

            glActiveTexture(GL_TEXTURE0 + texBuffer[2]->GetID());
            shader.SetInt("iChannel2", texBuffer[2]->GetID());
            glBindTexture(GL_TEXTURE_2D, texBuffer[2]->GetID());

            glActiveTexture(GL_TEXTURE0 + texBuffer[3]->GetID());
            shader.SetInt("iChannel3", texBuffer[3]->GetID());
            glBindTexture(GL_TEXTURE_2D, texBuffer[3]->GetID());
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            ft.Frame();
            frameNum += 1;
        }

        static void ErrorCheck() {
            GLenum error = glGetError();

            if (error == GL_NO_ERROR) {
                std::cout << "OK: all good" << std::endl;
            } else {
                std::cout << "ERROR: ";
            }

            if (error == GL_INVALID_OPERATION) {
                std::cout << "INVALID OPERATION" << std::endl;
            } else if (error == GL_INVALID_VALUE) {
                std::cout << "NO VALUE" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                std::cout << "NO MEMORY" << std::endl;
            } else if (error == GL_STACK_OVERFLOW) {
                std::cout << "NO OVERFLOW" << std::endl;
            } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
                std::cout << "Invalid Framebuffer" << std::endl;
            } else if (error == GL_OUT_OF_MEMORY) {
                std::cout << "Out of Memory" << std::endl;
            }
        }

};