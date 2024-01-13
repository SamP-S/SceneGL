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
#include "la_extended.h"
using namespace LA;
#include "camera.hpp"
#include "model.hpp"
#include "frame.hpp"
#include "mesh_renderer.hpp"
#include "first_person.hpp"
#include "resource_manager.hpp"
#include "asset_manager.hpp"
#include "light_directional.hpp"
#include "filepath.hpp"
#include "json.hpp"
using namespace nlohmann;


class GraphicsEngine {

    public:
        void* context;
        WindowManager* window;
        frame_timer ft = frame_timer();
        Frame* frame = NULL;
        int width, height;
        Entity* rootEntity = NULL;
        Entity* workspaceCamera = NULL;

        FirstPersonController* fpc;
        Camera* camera;

        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frame = new Frame(window->width, window->height);
            frame->SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
             
            GL_Interface::BindFrameBufferObj(0);

            // load default shader(s)
            resourceShaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));

            // load default model(s)
            assetManager.Load("models/presets/cone.gltf");
            assetManager.Load("models/presets/cube.gltf");
            assetManager.Load("models/presets/cylinder.gltf");
            assetManager.Load("models/presets/dome.gltf");
            assetManager.Load("models/presets/ico_sphere.gltf");
            assetManager.Load("models/presets/plane.gltf");
            assetManager.Load("models/presets/prism.gltf");
            assetManager.Load("models/presets/sphere.gltf");

            /// TODO: load project shader(s)
            rootEntity = new Entity("scene", NULL);

            LoadScene("scene/Preset.scene");

            // Entity* cube = new Entity("cube", rootEntity);
            // rootEntity->AddChild(cube);
            // Component* c = new MeshRenderer(*cube, resourceMeshes.GetId("cube::0::Cube"));
            // cube->AddComponent(c);

            workspaceCamera = new Entity("Workspace Camera", NULL);
            camera = new Camera(*workspaceCamera);
            fpc = new FirstPersonController(*workspaceCamera);
            
            workspaceCamera->transform.SetPosition(vec3{-8,5,8});
            workspaceCamera->transform.SetRotation(vec3({PI/2, PI, 0.0f}));

            resourceShaders.Get("base")->Use();
            camera->SetResolution(width, height);
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
            delete frame;
        }

        /// TODO: Implement as recursive and implement FromJson & ToJson functions for everything
        bool LoadScene(std::string filepath) {
            std::ifstream inputFile(filepath);
            if (!inputFile.good()) {
                std::cout << "WARNING (Graphics): Scene file does not exist: " << filepath << std::endl;
            }
            if (!inputFile.is_open()) {
                std::cout << "WARNING (Graphics): Can't open file: " << filepath << std::endl;
                return false;
            }

            json jsonFile = json::parse(inputFile);
            for (const auto& entry : jsonFile) {
                // parse main structs
                std::string name = entry["name"];
                json transform = entry["transform"];
                json position = transform["position"];
                json rotation = transform["rotation"];
                json scale = transform["scale"];
                json components = entry["components"];
                // debug output
                std::cout << "Name: " << name << std::endl;
                std::cout << "Position: (" << position["x"] << ", " << position["y"] << ", " << position["z"] << ")" << std::endl;
                std::cout << "Rotation: (" << rotation["x"] << ", " << rotation["y"] << ", " << rotation["z"] << ")" << std::endl;
                std::cout << "Scale: (" << scale["x"] << ", " << scale["y"] << ", " << scale["z"] << ")" << std::endl;
                // create entity
                Entity* ent = new Entity(name, rootEntity);
                ent->transform.SetPosition(vec3({position["x"], position["y"], position["z"]}));
                ent->transform.SetRotation(vec3({rotation["x"], rotation["y"], rotation["z"]}));
                ent->transform.SetScale(vec3({scale["x"], scale["y"], scale["z"]}));
                rootEntity->AddChild(ent);
                // create component(s)
                for (const auto& component : components) {
                    if (component.find("directionalLight") != component.end()) {
                        json directionalLight = component["directionalLight"];
                        std::cout << "Directional Light: (" << directionalLight["r"] << ", " << directionalLight["g"] << ", " << directionalLight["b"] << ")" << std::endl;
                        Component* c = new DirectionalLight(*ent, vec3({directionalLight["r"], directionalLight["g"], directionalLight["b"]}));
                        ent->AddComponent(c);
                    }
                    else if (component.find("mesh") != component.end()) {
                        std::cout << "Mesh: " << component["mesh"] << std::endl;
                        ObjId resId = resourceMeshes.GetId(component["mesh"]);
                        Component* c = new MeshRenderer(*ent, resId);
                        ent->AddComponent(c);
                    }
                }

                std::cout << std::endl; // Separating each object for clarity
            }
            inputFile.close();
            return true;
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
        void SetViewport(int _width, int _height) {
            // protect against divide by 0 and no resoltuion
            if (_width == 0) {
                std::cout << "WARNING (Graphics): Trying to set width 0" << std::endl;
                _width = 1;
            }
                
            if (_height == 0) {
                std::cout << "WARNING (Graphics): Trying to set height 0" << std::endl;
                _height = 1;
            }
                
            this->width = _width;
            this->height = _height;
            float ratio = width / height;
        }

        void RenderObject(Entity* entity, mat4 root_trans = mat4()) {
            mat4 model = root_trans *  entity->transform.GetTransform();
            for (int i = 0; i < entity->GetNumChildren(); i++) {
                RenderObject(entity->GetChild(i), model);
            }

            // ensure mesh not empty
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0 )
                return; 

            resourceShaders.Get("base")->SetMat4("iModel", &model[0][0]);
            renderer->Render();
        }

        void Render() {
            frame->Bind();
            frame->Clear();
            
            GL_Interface::SetViewport(width, height);

            // GL_Interface::DisableFeature(FEATURE_DEPTH);
            // GL_Interface::DisableFeature(FEATURE_CULL);
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            GL_Interface::EnableFeature(FEATURE_CULL);
            GL_Interface::SetFrontFace(FRONT_CCW);

            resourceShaders.Get("base")->Use();

            camera->SetResolution(width, height);
            fpc->Update();

            std::vector<DirectionalLight*> dirLights = rootEntity->GetComponentsInChildren<DirectionalLight>();

            // vertex uniforms
            resourceShaders.Get("base")->SetMat4("iView", &fpc->view[0][0]);
            resourceShaders.Get("base")->SetMat4("iProjection", &camera->proj[0][0]);
            // fragment uniforms
            resourceShaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            resourceShaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            resourceShaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            resourceShaders.Get("base")->SetInt("iFrame", ft.GetFrameCount());
            resourceShaders.Get("base")->SetVec3("iCameraPosition", workspaceCamera->transform.GetPosition());

            for (int i = 0; i < dirLights.size(); i++) {
                if (i >= 4) {
                    std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
                    break;
                }
                resourceShaders.Get("base")->SetVec3("iDirectionalLights[0].direction", dirLights[i]->transform.GetForward());
                resourceShaders.Get("base")->SetVec3("iDirectionalLights[0].strength", dirLights[i]->GetStrength());
                resourceShaders.Get("base")->SetInt("iDirectionalLights[0].enabled", 1);
            }

            RenderObject(rootEntity);

            frame->Unbind();
            
            ft.Frame();
        }

};