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

#include "ngine/ngine.hpp"
#include "ngine/serializer.hpp"

#include "ecs/resource_manager.hpp"
#include "ecs/asset_manager.hpp"

#include "la_extended.h"

#include "core/frame_timer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/mesh.hpp"
#include "renderer/frame.hpp"
#include "renderer/editor_camera.hpp"


class GraphicsEngine {
    private:
        int _width, _height;

    public:
        frame_timer ft = frame_timer();
        Frame* frame = nullptr;
        EditorCamera editorCamera = EditorCamera();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

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

            LoadScene("scene/Preset.json");    

            // setup editor camera
            editorCamera.transform.position = LA::vec3{-8,5,8};
            editorCamera.transform.rotation = LA::vec3({PI/2, PI, 0.0f});
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

        void LoadScene(const std::string& filepath) {
            // Load Scene
            JsonSerializer js = JsonSerializer(scene);
            js.Deserialize(filepath);
        }

        void SaveScene(const std::string& filepath) {
            std::cout << "NOT IMPLEMENTED" << std::endl;
        }

        void ShaderDirectionalLights(Shader& shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<DirectionalLightComponent>();
            
            // directional lights
            if (entities.size() >= DIRECTIONAL_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
            }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                DirectionalLightComponent& dlc = entities[i].GetComponent<DirectionalLightComponent>();
                if (i < entities.size()) {
                    shader.SetVec3("iDirectionalLights" + index + ".direction", tc.GetForward());
                    shader.SetFloat("iDirectionalLights" + index + ".intensity", dlc.intensity);
                    shader.SetVec3("iDirectionalLights" + index + ".colour", dlc.colour);
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(Shader& shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<PointLightComponent>();

            // point lights
            if (entities.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                PointLightComponent& plc = entities[i].GetComponent<PointLightComponent>();
                if (i < entities.size()) {
                    shader.SetVec3("iPointLights" + index + ".position", tc.position);
                    shader.SetFloat("iPointLights" + index + ".intensity", plc.intensity);
                    shader.SetVec3("iPointLights" + index + ".colour", plc.colour);
                    shader.SetInt("iPointLights" + index + ".enabled", 1);
                } else {
                    shader.SetInt("iPointLights" + index + ".enabled", 0);
                }
            }
        }

        void SetupShader(Shader& shader) {
            shader.Use();
            // vertex uniforms
            shader.SetMat4("iView", &inverse(editorCamera.transform.GetTransform())[0][0]);
            shader.SetMat4("iProjection", &(editorCamera.GetProjection())[0][0]);
            // fragment uniforms
            shader.SetVec3("iResolution", _width, _height, 1.0f);
            shader.SetFloat("iTime", ft.GetTotalElapsed());
            shader.SetFloat("iTimeDelta", ft.GetFrameElapsed());
            shader.SetInt("iFrame", ft.GetFrameCount());
            shader.SetVec3("iCameraPosition", editorCamera.transform.position); 
 
            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);     
        }

        void Draw(MeshRendererComponent& renderer, bool wireframe=false) {
            if (renderer.mesh != 0) {
                Mesh* mesh = resourceMeshes.Get(renderer.mesh);
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

        void RenderObject(Entity entity, mat4 root_trans = mat4(), Shader* shader=NULL, bool wireframe=false) {
            TransformComponent& tc = entity.GetComponent<TransformComponent>();
            mat4 model = root_trans *  tc.GetTransform();

            // check entity has renderer
            if (!entity.HasComponent<MeshRendererComponent>())
                return;
            // check mesh is not empty
            MeshRendererComponent& mrc = entity.GetComponent<MeshRendererComponent>();
            if (mrc.mesh == 0)
                return;
            shader->SetMat4("iModel", &model[0][0]);
            Draw(mrc, wireframe);
        }

        void Render() {
            frame->Bind();
            frame->Clear();
            
            GL_Interface::SetViewport(_width, _height);
            GL_Interface::SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

            for (auto& shaderPair : resourceShaders) {
                SetupShader(*shaderPair.second);
            }

            Shader* baseShader = resourceShaders.Get("base");
            Shader* lightingShader = resourceShaders.Get("lighting");

            editorCamera.Update();

            std::vector<Entity> renderables = scene->GetEntitiesWith<MeshRendererComponent>();
            for (auto& ent : renderables) {
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
            }

            frame->Unbind();
            
            ft.Frame();
        }

};