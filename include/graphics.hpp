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
#include "entity.hpp"
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
        EntityId worldSelected = -1;
        std::vector<EntityId> world = std::vector<EntityId>();
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

            resourceModels.Add(new Model("test1", "/root/SceneGL/models/cottage/Cottage_FREE.obj"));
            resourceModels.Add(new Model("test2", "/root/SceneGL/models/cottage/Cottage_FREE.dae"));
            resourceModels.Add(new Model("test3", "/root/SceneGL/models/cottage/Cottage_FREE.3DS"));  
            resourceModels.Add(new Model("test4", "/root/SceneGL/models/cottage/Cottage_FREE.fbx"));
            resourceModels.Add(new Model("test5", "/root/SceneGL/models/cottage/Cottage_FREE.stl"));

            resourceShaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));
            resourceMeshes.Add(new Mesh("vertex_cube", cubeVertices, cubeColours, cubeIndicies));
            resourceMeshes.Add(new Mesh("vertex_quad", quadVertices));

            resourceEntities.Add(new Entity("cube0", NULL, resourceMeshes.GetId("vertex_cube")));
            resourceEntities.Add(new Entity("cube1", resourceEntities.GetId("cube0"), resourceMeshes.GetId("vertex_cube")));
            resourceEntities.Get(0)->AddChild(1);
            resourceEntities.Add(new Entity("Cottage", NULL, resourceMeshes.GetId("test1::Cottage_Free")));

            world.push_back(resourceEntities.GetId("cube0"));
            world.push_back(resourceEntities.GetId("Cottage"));

            resourceEntities.Get(0)->trans.Translate(0.0f, 0.0f, -10.0f);
            resourceEntities.Get(0)->trans.SetScale({0.5f, 0.5f, 0.5f});
            resourceEntities.Get(1)->trans.Translate(0.0f, 0.0f, 10.0f);
            resourceEntities.Get(1)->trans.SetScale({0.8f, 0.8f, 0.8f});
            resourceEntities.Get(2)->trans.Translate({5.0f, 0.0f, -5.0f});
            resourceEntities.Get(2)->trans.SetScale(0.1f);

            resourceShaders.Get("base")->Use();
            camera.SetProjection(45.0f, float(width), (float)height, 0.1f, 100.0f);
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

        void RenderObject(int entityId, mat4 root_trans = mat4()) {
            Entity* ent = resourceEntities.Get(entityId);
            mat4 model = root_trans *  ent->trans.GetTransform();
            for (int i = 0; i < ent->GetNumChildren(); i++) {
                RenderObject(ent->GetChild(i), model);
            }

            if (ent->GetMesh() < 0 )
                return; 
            resourceShaders.Get("base")->SetMat4("iModel", &model[0][0]);
            
            Mesh* mesh = resourceMeshes.Get(ent->GetMesh());
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
            resourceShaders.Get("base")->SetMat4("iView", &camera.view[0][0]);
            resourceShaders.Get("base")->SetMat4("iProjection", &camera.proj[0][0]);

            resourceShaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            resourceShaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            resourceShaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            resourceShaders.Get("base")->SetInt("iFrame", _frameNum);

            RenderObject(world.at(0));
            RenderObject(world.at(1));

            GL_Interface::BindFrameBufferObj(0);
            
            ft.Frame();
            _frameNum += 1;
        }

};