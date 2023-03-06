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
#include "object.hpp"
#include "la_extended.hpp"

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
        int worldSelected = 0;
        std::vector<Object*> world = std::vector<Object*>();
        mat4 view = mat4();

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
            GL_Interface::SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);

            shaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));
            meshes.Add(new Mesh("cube", cubeVertices, cubeColours, cubeIndicies));
            meshes.Add(new Mesh("quad", quadVertices));

            world.push_back(new Object("cube0", NULL, 0));
            world.push_back(new Object("cube1", NULL, 0));
            world.at(0)->trans.Translate(0.0f, 0.0f, -10.0f);
            world.at(0)->trans.SetScale({0.5f, 0.5f, 0.5f});
            world.at(1)->trans.Translate(0.0f, 0.0f, 10.0f);
            world.at(1)->trans.SetScale({0.8f, 0.8f, 0.8f});
            shaders.Get("base")->Use();
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
            for (auto obj : world) {
                delete obj;
            }
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

        void RenderObject(Object* obj) {
            mat4 pos = Translate(obj->trans.GetPosition());
            mat4 rot = Rotate(obj->trans.GetRotation());
            mat4 scl = Scale(obj->trans.GetScale());
            // std::cout << "pos:\n" << pos << std::endl;
            // std::cout << "rot:\n" << rot << std::endl;
            // std::cout << "scl:\n" << scl << std::endl;
            mat4 model = pos * rot * scl * mat4();
            // std::cout << "model:\n" << model << std::endl;
            shaders.Get("base")->SetMat4("iModel", &model[0][0]);
            
            Mesh* mesh = meshes.Get(obj->GetMesh());
            GL_Interface::BindVertexArrayObject(mesh->vao);
            GL_Interface::DrawElements(DRAW_TRIANGLES, mesh->GetIndiciesSize(), TYPE_UINT);
            // GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, 6);
        }

        void Render() {
            GL_Interface::BindFrameBufferObj(fbo);
            
            GL_Interface::SetViewport(width, height);
            /* Clear The Screen And The Depth Buffer */
            GL_Interface::ClearColourDepth();
            
            // GL_Interface::DisableFeature(FEATURE_DEPTH);
            GL_Interface::DisableFeature(FEATURE_CULL);
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            // GL_Interface::EnableFeature(FEATURE_CULL);
            GL_Interface::SetFrontFace(FRONT_CW);

            shaders.Get("base")->Use();

            view = Perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
            // std::cout << "view:\n" << view << std::endl;
            shaders.Get("base")->SetMat4("iView", &view[0][0]);

            shaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            shaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            shaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shaders.Get("base")->SetInt("iFrame", _frameNum);

            // world.at(0)->trans.Rotate(PI/580, PI/720, 0.0f);
            // float tmp = 0.4 + 0.1*sin(ft.GetTotalElapsed()*PI/2);
            // world.at(0)->trans.SetScale({tmp, tmp, tmp});
            // world.at(0)->trans.SetPosition({0.0f, 0.0f, -5 + 2*sin(ft.GetTotalElapsed()*PI/2)});
            RenderObject(world.at(0));
            RenderObject(world.at(1));

            GL_Interface::BindFrameBufferObj(0);
            
            ft.Frame();
            _frameNum += 1;
        }

};