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

#include "ecs/entity.hpp"
#include "core/filepath.hpp"
#include "ecs/scene.hpp"
#include "ecs/resource_manager.hpp"
#include "ecs/asset_manager.hpp"

#include "la_extended.h"
using namespace LA;
#include "json.hpp"
using namespace nlohmann;

#include "core/frame_timer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/mesh.hpp"

#include "renderer/camera.hpp"
#include "renderer/model.hpp"
#include "renderer/frame.hpp"
#include "renderer/mesh_renderer.hpp"
#include "renderer/light_directional.hpp"
#include "renderer/light_point.hpp"
#include "scripting/first_person.hpp"



class GraphicsEngine {
    private:
        int _width, _height;

    public:
        frame_timer ft = frame_timer();
        Frame* frame = nullptr;
        Scene* scene = nullptr;
        Entity* sceneCam = nullptr;

        GraphicsEngine(int width, int height) :
        _width(width), _height(height) { }
        ~GraphicsEngine() {
            delete frame;
        }

        void Initialise() {
            SetViewport(_width, _height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frame = new Frame(_width, _height);
            frame->SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
             
            GL_Interface::BindFrameBufferObj(0);

            // load default model(s)
            assetManager.Load("models/presets/cone.gltf");
            assetManager.Load("models/presets/cube.gltf");
            assetManager.Load("models/presets/cylinder.gltf");
            assetManager.Load("models/presets/dome.gltf");
            assetManager.Load("models/presets/ico_sphere.gltf");
            assetManager.Load("models/presets/plane.gltf");
            assetManager.Load("models/presets/prism.gltf");
            assetManager.Load("models/presets/sphere.gltf");
            // load default shader(s)
            assetManager.Load("shaders/base.vert");
            assetManager.Load("shaders/base.frag");
            assetManager.Load("shaders/lighting.vert");
            assetManager.Load("shaders/lighting.frag");
            resourceShaders.Create("base", resourceShaderStages.Get("base_vert"), resourceShaderStages.Get("base_frag"));
            resourceShaders.Create("lighting", resourceShaderStages.Get("lighting_vert"), resourceShaderStages.Get("lighting_frag"));

            /// TODO: load project shader(s)
            scene = new Scene();
            scene->FromJson(LoadJson("scene/Preset.json"));

            sceneCam = new Entity("Scene Camera");
            sceneCam->AddComponent<Camera>();
            sceneCam->AddComponent<FirstPersonController>();
            sceneCam->transform->SetPosition(vec3{-8,5,8});
            sceneCam->transform->SetRotation(vec3({PI/2, PI, 0.0f}));

            resourceShaders.Get("base")->Use();
            sceneCam->GetComponent<Camera>()->SetResolution(_width, _height);
        }

        json LoadJson(std::string filepath) {
            std::ifstream inputFile(filepath);
            if (!inputFile.good()) {
                std::cout << "WARNING (Graphics): Scene file does not exist: " << filepath << std::endl;
                return json();
            }
            if (!inputFile.is_open()) {
                std::cout << "WARNING (Graphics): Can't open file: " << filepath << std::endl;
                return json();
            }
            json jsonFile = json::parse(inputFile);
            inputFile.close();
            return jsonFile;
        }

        bool SaveJson(std::string filepath, json jsonData) {
            std::ofstream outputFile(filepath);
            if (!outputFile.is_open()) {
                std::cout << "WARNING (Graphics): Can't open file: " << filepath << std::endl;
                return false;
            }
            outputFile << jsonData.dump(4); // Write the JSON data to the file with indentation of 4 spaces
            outputFile.close();
            return true;
        }

        void LoadScene(std::string filepath) {
            if (scene != nullptr)
                delete scene;
            scene = new Scene();
            scene->FromJson(LoadJson(filepath));
        }

        void SaveScene(std::string filepath) {
            json j = json();
            if (scene != nullptr) {
                j = scene->ToJson();
            }
            SaveJson(filepath, j);
        }
        
        // function to reset our viewport after a window resize
        void SetViewport(int width, int height) {
            // protect against divide by 0 and no resoltuion
            if (width == 0) {
                std::cout << "WARNING (Graphics): Trying to set width 0" << std::endl;
                width = 1;
            }
                
            if (height == 0) {
                std::cout << "WARNING (Graphics): Trying to set height 0" << std::endl;
                height = 1;
            }
                
            _width = width;
            _height = height;
            float ratio = width / height;
        }

        void ShaderDirectionalLights(Shader& shader, const std::vector<DirectionalLight*>& dirLights) {
            // directional lights
            if (dirLights.size() >= DIRECTIONAL_LIGHT_MAX) {
                    std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
                }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < dirLights.size()) {
                    shader.SetVec3("iDirectionalLights" + index + ".direction", dirLights[i]->transform->GetForward());
                    shader.SetFloat("iDirectionalLights" + index + ".intensity", dirLights[i]->GetIntensity());
                    shader.SetVec3("iDirectionalLights" + index + ".colour", dirLights[i]->GetColour());
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(Shader& shader, const std::vector<PointLight*>& pointLights) {
            // point lights
            if (pointLights.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < pointLights.size()) {
                    shader.SetVec3("iPointLights" + index + ".position", pointLights[i]->transform->GetPosition());
                    shader.SetFloat("iPointLights" + index + ".intensity", pointLights[i]->GetIntensity());
                    shader.SetVec3("iPointLights" + index + ".colour", pointLights[i]->GetColour());
                    shader.SetInt("iPointLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iPointLights" + index + ".enabled", 0);
                }
            }
        }

        void SetupShader(Shader& shader, const std::vector<DirectionalLight*>& dirLights, const std::vector<PointLight*>& pointLights) {
            shader.Use();
            // vertex uniforms
            shader.SetMat4("iView", &inverse(sceneCam->transform->GetTransform())[0][0]);
            shader.SetMat4("iProjection", &sceneCam->GetComponent<Camera>()->proj[0][0]);
            // fragment uniforms
            shader.SetVec3("iResolution", _width, _height, 1.0f);
            shader.SetFloat("iTime", ft.GetTotalElapsed());
            shader.SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader.SetInt("iFrame", ft.GetFrameCount());
            shader.SetVec3("iCameraPosition", sceneCam->transform->GetPosition()); 
 
            ShaderDirectionalLights(shader, dirLights);
            ShaderPointLights(shader, pointLights);     
        }

        /// TODO: Migrate to transform
        mat4 GetCoordinateSystem(Entity* entCoord) {
            if (entCoord == nullptr)
                return mat4();
            mat4 result = entCoord->transform->GetTransform();
            while (true) {
                if (entCoord->GetParent() != nullptr)
                    break;
                entCoord = entCoord->GetParent();
                result = entCoord->transform->GetTransform() * result;
            }
            return result;
        }

        void Draw(MeshRenderer& renderer, bool wireframe=false) {
            ObjId meshId = renderer.GetMeshId();
            if (meshId != 0) {
                Mesh* mesh = resourceMeshes.Get(meshId);
                if (mesh == NULL || !mesh->GetIsGenerated()) {
                    std::cout << "WARNING (Graphics): Attempting to render a bad mesh." << std::endl;
                    return;
                }

                GL_Interface::BindVertexArrayObject(mesh->vao);
                if (wireframe) {
                    GL_Interface::PolygonMode(POLYGON_LINE);
                    GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, mesh->GetVerticesSize());
                    GL_Interface::DrawElements(DRAW_TRIANGLES, mesh->GetIndiciesSize(), TYPE_UINT);
                    GL_Interface::PolygonMode(POLYGON_FILL);
                } else {
                    GL_Interface::DrawArrays(DRAW_TRIANGLES, 0, mesh->GetVerticesSize());
                    GL_Interface::DrawElements(DRAW_TRIANGLES, mesh->GetIndiciesSize(), TYPE_UINT);
                }
                return;
            }
        }

        void RenderSelected(Entity* entity, Shader* shader=NULL, bool wireframe=false) {
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0)
                return;
            mat4 model = GetCoordinateSystem(entity);
            shader->SetMat4("iModel", &model[0][0]);
            Draw(*renderer);
        }

        void RenderObject(Entity* entity, mat4 root_trans = mat4(), Shader* shader=NULL, bool wireframe=false) {
            mat4 model = root_trans *  entity->transform->GetTransform();
            for (int i = 0; i < entity->GetNumChildren(); i++) {
                RenderObject(entity->GetChild(i), model, shader, wireframe);
            }

            // ensure mesh not empty
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0)
                return; 
            shader->SetMat4("iModel", &model[0][0]);
            Draw(*renderer, wireframe);
        }

        void Render() {
            frame->Bind();
            frame->Clear();
            
            GL_Interface::SetViewport(_width, _height);
            GL_Interface::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

            std::vector<DirectionalLight*> dirLights = scene->at(0)->GetComponentsInChildren<DirectionalLight>();
            std::vector<PointLight*> pointLights = scene->at(0)->GetComponentsInChildren<PointLight>();    

            for (auto& shaderPair : resourceShaders) {
                SetupShader(*shaderPair.second, dirLights, pointLights);
            }

            Shader* baseShader = resourceShaders.Get("base");
            Shader* lightingShader = resourceShaders.Get("lighting");

            sceneCam->GetComponent<Camera>()->SetResolution(_width, _height);
            sceneCam->GetComponent<FirstPersonController>()->Update();

            for (auto& ent : *scene) {
                // draw lit
                GL_Interface::EnableFeature(FEATURE_DEPTH);
                GL_Interface::EnableFeature(FEATURE_CULL);
                GL_Interface::SetFrontFace(FRONT_CCW);
                lightingShader->Use();
                RenderObject(ent, mat4(), lightingShader, false);

                // draw wireframe
                GL_Interface::EnableFeature(FEATURE_DEPTH);
                GL_Interface::EnableFeature(FEATURE_CULL);
                baseShader->Use();
                RenderObject(ent, mat4(), baseShader, true);

                // draw selected
                // baseShader->Use();
                // RenderSelected()
            }

            

            frame->Unbind();
            
            ft.Frame();
        }

};