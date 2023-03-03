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
#include "mesh.hpp"
#include "resource_manager.hpp"

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

    public:
        void* context;
        WindowManager* window;
        frame_timer ft = frame_timer();
        ResourceManager<Shader> shaders = ResourceManager<Shader>();
        ResourceManager<Mesh> meshes = ResourceManager<Mesh>();
        int width, height;
        uint32_t fbo, texColour, texDepthStencil;
        unsigned int _frameNum = 0;
        

        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            fbo = GL_Interface::GenFrameBufferObj();
            texColour = GL_Interface::GenTexture2D(width, height);
            GL_Interface::BindFrameBufferTexture2D(fbo, texColour);
            texDepthStencil = GL_Interface::GenStencil2D(width, height);
            GL_Interface::BindFrameBufferStencil2D(fbo, texDepthStencil);
             
            GL_Interface::BindFrameBufferObj(0);

            shaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));
            meshes.Add(new Mesh("cube", cubeVertices, cubeColours, cubeIndicies));
            meshes.Add(new Mesh("quad", quadVertices));

            shaders.Get("base")->Use();
            GL_Interface::SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
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


        void Render() {
            GL_Interface::BindFrameBufferObj(fbo);
            
            GL_Interface::SetViewport(width, height);
            /* Clear The Screen And The Depth Buffer */
            GL_Interface::ClearColourDepth();
            GL_Interface::BindVertexArrayObject(meshes.Get("cube")->vao);
            // GL_Interface::DisableFeature(FEATURE_DEPTH);
            GL_Interface::DisableFeature(FEATURE_CULL);
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            // GL_Interface::EnableFeature(FEATURE_CULL);
            GL_Interface::SetFrontFace(FRONT_CW);

            shaders.Get("base")->Use();

            shaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            shaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            shaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shaders.Get("base")->SetInt("iFrame", _frameNum);

            GL_Interface::DrawElements(DRAW_TRIANGLES, cubeIndicies.size(), TYPE_UINT);
            // GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, 6);

            GL_Interface::BindFrameBufferObj(0);
            
            ft.Frame();
            _frameNum += 1;
        }

};