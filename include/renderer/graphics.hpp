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
#include "serializer/serializer.hpp"

#include "la_extended.h"

#include "core/frame_timer.hpp"

#include "platform/opengl/opengl.hpp"
#include "platform/opengl/opengl_shader.hpp"
#include "platform/opengl/opengl_shader_source.hpp"
#include "platform/opengl/opengl_frame_buffer.hpp"
#include "platform/opengl/opengl_mesh.hpp"

#include "renderer/editor_camera.hpp"

#include "renderer/model_loader.hpp"
#include "renderer/shader_loader.hpp"

class GraphicsEngine {
    private:
        int _width, _height;

    public:
        frame_timer ft = frame_timer();
        EditorCamera editorCamera = EditorCamera();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        Ngine::AssetManager& assetManager = Ngine::AssetManager::Instance();
        Ngine::AssetLoaderManager loaderManager = Ngine::AssetLoaderManager();
        std::shared_ptr<FrameBuffer> frameBuffer = nullptr;

        GraphicsEngine(int width, int height) :
        _width(width), _height(height) { }
        ~GraphicsEngine() {}

        void Initialise() {
            SetViewport(_width, _height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frameBuffer = assetManager.CreateAsset<OpenGLFrameBuffer>("FBO", _width, _height);
            frameBuffer->SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

            // add loaders to asset libary
            loaderManager.AddLoader(new ModelLoader());
            loaderManager.AddLoader(new ShaderLoader());

            // load default model(s)
            loaderManager.Load("models/presets/cone.gltf");
            loaderManager.Load("models/presets/cube.gltf");
            loaderManager.Load("models/presets/cylinder.gltf");
            loaderManager.Load("models/presets/dome.gltf");
            loaderManager.Load("models/presets/ico_sphere.gltf");
            loaderManager.Load("models/presets/plane.gltf");
            loaderManager.Load("models/presets/prism.gltf");
            loaderManager.Load("models/presets/sphere.gltf");

            // load default shader source(s)
            loaderManager.Load("shaders/base.vert");
            loaderManager.Load("shaders/base.frag");
            loaderManager.Load("shaders/lighting.vert");
            loaderManager.Load("shaders/lighting.frag");

            // load default shader(s)
            std::shared_ptr<Shader> base = assetManager.CreateAsset<OpenGLShader>(
                "base",
                assetManager.GetAsset<OpenGLShaderSource>("base_vert"),
                assetManager.GetAsset<OpenGLShaderSource>("base_frag")
            );
            std::shared_ptr<Shader> lighting = assetManager.CreateAsset<OpenGLShader>(
                "lighting",
                assetManager.GetAsset<OpenGLShaderSource>("lighting_vert"),
                assetManager.GetAsset<OpenGLShaderSource>("lighting_frag")
            );

            // load material(s)
            std::shared_ptr<Material> material = assetManager.CreateAsset<OpenGLMaterial>(
                "default-material"
            );
            material->SetProperty("colour", vec4(1.0f));
            material->shader = lighting;

            // load scene
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

        // Load Scene from JSON
        void LoadScene(const std::string& filepath) {
            JsonSerializer js = JsonSerializer(scene);
            js.Deserialize(filepath);
        }

        // Save scene to JSON
        void SaveScene(const std::string& filepath) {
            JsonSerializer js = JsonSerializer(scene);
            js.Serialize(filepath);
        }

        void RenderObject(Entity entity, mat4 root_trans = mat4(), bool wireframe=false) {
            TransformComponent& tc = entity.GetComponent<TransformComponent>();
            mat4 model = root_trans *  tc.GetTransform();

            // check entity has renderer
            if (!entity.HasComponent<MeshRendererComponent>())
                return;

            // check meshrenderer mesh is not none
            MeshRendererComponent& mrc = entity.GetComponent<MeshRendererComponent>();
            std::shared_ptr<Mesh> mesh = mrc.mesh;
            std::shared_ptr<Material> material = mrc.material;
            if (mesh == nullptr) {
                std::cout << "WARNING (Graphics): Attempting to draw null mesh." << std::endl;
                return;
            }
            if (material == nullptr) {
                std::cout << "WARNING (Graphics): Attempting to draw null material." << std::endl;
                return;
            }

            std::shared_ptr<Shader> shader;
            if (wireframe) {
                shader = assetManager.GetAsset<OpenGLShader>("base");
            } else {
                shader = material->shader;
            }
            if (shader == nullptr) {
                std::cout << "WARNING (Graphics): Attempting to draw mesh with null shader." << std::endl;
                return;
            }
            
            // pass model coordinate space
            material->Bind();
            // rebind for wireframe
            shader->Bind();
            shader->SetMat4("uModel", &model[0][0]);

            // draw mesh
            mrc.mesh->Draw();
        }

        void Render(bool wireframe=false) {
            frameBuffer->Bind();
            frameBuffer->Clear();
            editorCamera.Update();
            SetupShaders();

            std::vector<Entity> renderables = scene->GetEntitiesWith<MeshRendererComponent>();
            for (auto& ent : renderables) {
                // draw lit
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
                glFrontFace(GL_CCW);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                RenderObject(ent, mat4(), false);

                // draw wireframe
                glEnable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);
                glLineWidth(3.0f);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                RenderObject(ent, mat4(), true);
            }

            frameBuffer->Unbind();
            ft.Frame();
        }

    private:
        void SetupShaders() {
            std::vector<std::shared_ptr<Ngine::Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
            for (auto asset : shaders) {
                SetupShader(std::dynamic_pointer_cast<Shader>(asset));
            }
        }

        void SetupShader(std::shared_ptr<Shader> shader) {
            shader->Bind();
            // vertex uniforms
            shader->SetMat4("uView", inverse(editorCamera.transform.GetTransform()));
            shader->SetMat4("uProjection", editorCamera.GetProjection());
            // fragment uniforms
            shader->SetVec3("uResolution", _width, _height, 1.0f);
            shader->SetFloat("uTime", ft.GetTotalElapsed());
            shader->SetFloat("uTimeDelta", ft.GetFrameElapsed());
            shader->SetInt("uFrame", ft.GetFrameCount());
            shader->SetVec3("uCameraPosition", editorCamera.transform.position); 

            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);   
        }

        void ShaderDirectionalLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<DirectionalLightComponent>();
            
            // directional lights
            if (entities.size() >= DIRECTIONAL_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many directional lights, only first 4 will be used" << std::endl;
            }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    DirectionalLightComponent& dlc = entities[i].GetComponent<DirectionalLightComponent>();
                    shader->SetVec3("uDirectionalLights" + index + ".direction", tc.GetForward());
                    shader->SetFloat("uDirectionalLights" + index + ".intensity", dlc.intensity);
                    shader->SetVec3("uDirectionalLights" + index + ".colour", dlc.colour);
                    shader->SetInt("uDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("uDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = scene->GetEntitiesWith<PointLightComponent>();

            // point lights
            if (entities.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Graphics): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    PointLightComponent& plc = entities[i].GetComponent<PointLightComponent>();
                    shader->SetVec3("uPointLights" + index + ".position", tc.position);
                    shader->SetFloat("uPointLights" + index + ".intensity", plc.intensity);
                    shader->SetVec3("uPointLights" + index + ".colour", plc.colour);
                    shader->SetInt("uPointLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("uPointLights" + index + ".enabled", 0);
                }
            }
        }

};