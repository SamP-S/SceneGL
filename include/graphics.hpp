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
#include "object.hpp"
#include "la_extended.hpp"
#include "camera.hpp"
#include "model.hpp"

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
        int width, height;
        uint32_t fbo, texColour, texDepthStencil;
        unsigned int _frameNum = 0;
        int worldSelected = 0;
        std::vector<Object*> world = std::vector<Object*>();
        mat4 proj = mat4();
        Camera camera = Camera();

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

            Model model1 = Model("test1", "/root/SceneGL/models/cottage/Cottage_FREE.obj");
            Model model2 = Model("test2", "/root/SceneGL/models/cottage/Cottage_FREE.dae");
            Model model3 = Model("test3", "/root/SceneGL/models/cottage/Cottage_FREE.3DS");  
            Model model4 = Model("test4", "/root/SceneGL/models/cottage/Cottage_FREE.fbx");
            Model model5 = Model("test5", "/root/SceneGL/models/cottage/Cottage_FREE.stl");

            resourceShaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));
            resourceMeshes.Add(new Mesh("vertex_cube", cubeVertices, cubeColours, cubeIndicies));
            resourceMeshes.Add(new Mesh("vertex_quad", quadVertices));

            world.push_back(new Object("cube0", NULL, resourceMeshes.GetId("vertex_cube")));
            world.push_back(new Object("cube1", NULL, resourceMeshes.GetId("vertex_cube")));
            world.push_back(new Object("Cottage", NULL, resourceMeshes.GetId("test1::Cottage_Free")));
            world.at(0)->trans.Translate(0.0f, 0.0f, -10.0f);
            world.at(0)->trans.SetScale({0.5f, 0.5f, 0.5f});
            world.at(1)->trans.Translate(0.0f, 0.0f, 10.0f);
            world.at(1)->trans.SetScale({0.8f, 0.8f, 0.8f});
            world.at(2)->trans.Translate({5.0f, 0.0f, -5.0f});
            world.at(2)->trans.SetScale(0.1f);
            resourceShaders.Get("base")->Use();
            camera.SetProjection(45.0f, float(width), (float)height, 0.1f, 100.0f);
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
            if (obj->GetMesh() < 0 )
                return; 
            mat4 model = obj->trans.GetTransform();
            resourceShaders.Get("base")->SetMat4("iModel", &model[0][0]);
            
            Mesh* mesh = resourceMeshes.Get(obj->GetMesh());
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

            resourceShaders.Get("base")->Use();

            camera.SetProjection(45.0f, float(width), (float)height, 0.1f, 100.0f);
            camera.Update();
            // std::cout << "camera:\n" << camera.proj << std::endl;
            // std::cout << "proj:\n" << proj << std::endl;
            resourceShaders.Get("base")->SetMat4("iView", &camera.view[0][0]);
            resourceShaders.Get("base")->SetMat4("iProjection", &camera.proj[0][0]);

            resourceShaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            resourceShaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            resourceShaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            resourceShaders.Get("base")->SetInt("iFrame", _frameNum);

            RenderObject(world.at(0));
            RenderObject(world.at(1));
            RenderObject(world.at(2));

            GL_Interface::BindFrameBufferObj(0);
            
            ft.Frame();
            _frameNum += 1;
        }

};