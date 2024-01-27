#pragma once

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

#include "la_extended.h"

#include "ngine/ngine.hpp"
#include "serializer/serializer.hpp"

#include "platform/opengl/opengl.hpp"
#include "platform/opengl/opengl_shader.hpp"
#include "platform/opengl/opengl_shader_source.hpp"
#include "platform/opengl/opengl_mesh.hpp"

#include "renderer/components.hpp"
#include "renderer/model_loader.hpp"
#include "renderer/shader_loader.hpp"
#include "renderer/editor_camera.hpp"

//// TODO:
// Might need to pass scene in every frame
// Detach debug tools from the renderer
// Extract to platform/opengl specific
// remove editor camera from renderer
// per frame operations should be detached from render scene calls

enum DrawMode {
    NONE = 0,
    POINTS,
    LINES,
    FILL
};


class Renderer {
    public:
        // grab singleton references
        AssetManager& assetManager = AssetManager::Instance();
        AssetLoaderManager& loaderManager = AssetLoaderManager::Instance();
        

        void Initialise() {
            // add loaders to asset libary
            loaderManager.AddLoader(new ModelLoader());
            loaderManager.AddLoader(new ShaderLoader());

            // load default model(s)
            loaderManager.Load("marathon/assets/models/presets/cone.gltf");
            loaderManager.Load("marathon/assets/models/presets/cube.gltf");
            loaderManager.Load("marathon/assets/models/presets/cylinder.gltf");
            loaderManager.Load("marathon/assets/models/presets/dome.gltf");
            loaderManager.Load("marathon/assets/models/presets/ico_sphere.gltf");
            loaderManager.Load("marathon/assets/models/presets/plane.gltf");
            loaderManager.Load("marathon/assets/models/presets/prism.gltf");
            loaderManager.Load("marathon/assets/models/presets/sphere.gltf");

            // load default shader source(s)
            loaderManager.Load("marathon/assets/shaders/base.vert");
            loaderManager.Load("marathon/assets/shaders/base.frag");
            loaderManager.Load("marathon/assets/shaders/lighting.vert");
            loaderManager.Load("marathon/assets/shaders/lighting.frag");

            // load default shader(s)
            std::shared_ptr<Shader> base = assetManager.CreateAsset<OpenGLShader>(
                "base",
                assetManager.FindAsset<OpenGLShaderSource>("base_vert"),
                assetManager.FindAsset<OpenGLShaderSource>("base_frag")
            );
            std::shared_ptr<Shader> lighting = assetManager.CreateAsset<OpenGLShader>(
                "lighting",
                assetManager.FindAsset<OpenGLShaderSource>("lighting_vert"),
                assetManager.FindAsset<OpenGLShaderSource>("lighting_frag")
            );

            // load material(s)
            std::shared_ptr<Material> material = assetManager.CreateAsset<OpenGLMaterial>(
                "default-material"
            );
            material->SetProperty("colour", vec4(1.0f));
            material->shader = lighting;
        }

        void OnWindowResize(uint32_t width, uint32_t height) {
            glViewport(0, 0, width, height);
        }

        void RenderObject(Entity entity, DrawMode m) {
            // check entity has renderer
            if (!entity.HasComponent<MeshRendererComponent>())
                return;

            TransformComponent& tc = entity.GetComponent<TransformComponent>();
            mat4 model = tc.GetTransform();

            // check meshrenderer mesh is not none
            MeshRendererComponent& mrc = entity.GetComponent<MeshRendererComponent>();
            std::shared_ptr<Mesh> mesh = mrc.mesh;
            std::shared_ptr<Material> material = mrc.material;
            if (mesh == nullptr || material == nullptr) {
                return;
            }

            std::shared_ptr<Shader> shader;
            if (m != DrawMode::FILL) {
                shader = assetManager.FindAsset<OpenGLShader>("base");
            } else {
                shader = material->shader;
            }
            if (!material->IsUsable()) {
                // std::cout << "WARNING (Renderer): Attempting to draw mesh with null shader." << std::endl;
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

        void SetDrawMode(DrawMode m) {
            switch(m) {
                case DrawMode::NONE:
                    std::cout << "ERROR (Renderer): Invalid DrawMode." << std::endl;
                    break;
                case DrawMode::POINTS:
                    {
                        // draw points
                        glEnable(GL_DEPTH_TEST);
                        glDisable(GL_CULL_FACE);
                        glPointSize(5.0f);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                    break;
                case DrawMode::LINES:
                    {
                        // draw wireframe
                        glEnable(GL_DEPTH_TEST);
                        glDisable(GL_CULL_FACE);
                        glLineWidth(3.0f);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                    break;
                case DrawMode::FILL:
                    {
                        // draw lit
                        glEnable(GL_DEPTH_TEST);
                        glEnable(GL_CULL_FACE);
                        glFrontFace(GL_CCW);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                    break;
            }
        }

        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

        void RenderSceneByEditorCamera(std::shared_ptr<Scene> scene, EditorCamera& editorCamera, DrawMode m=DrawMode::FILL) {
            std::vector<std::shared_ptr<Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
            for (auto asset : shaders) {
                SetPerEditorCameraUniforms(std::dynamic_pointer_cast<Shader>(asset), editorCamera);
            }
            RenderScene(scene, m);
        }

        void RenderSceneByCamera(std::shared_ptr<Scene> scene, Entity& camera, DrawMode m=DrawMode::FILL) {
            std::cout << "Render scene by camera not implemented" << std::endl;
            return;
        }

        void RenderScene(std::shared_ptr<Scene> scene, DrawMode m=DrawMode::FILL) {
            _scene = scene;
            std::vector<std::shared_ptr<Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
            for (auto asset : shaders) {
                SetPerFrameUniforms(std::dynamic_pointer_cast<Shader>(asset));
                SetPerSceneUniforms(std::dynamic_pointer_cast<Shader>(asset));
            }

            std::vector<Entity> renderables = _scene->GetEntitiesWith<MeshRendererComponent>();
            for (auto& ent : renderables) {
                SetDrawMode(m);
                RenderObject(ent, m);
            }
        }

        // Update frame time info
        void OnUpdate(double dt) {
            _timeElapsed += dt;
            _timeDelta = dt;
            _frameCount++;

            // pass to shaders
            std::vector<std::shared_ptr<Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
            for (auto asset : shaders) {
                SetPerFrameUniforms(std::dynamic_pointer_cast<Shader>(asset));
            }
        }

    // delete copy and assign operators
    // should always get instance from class::instance func
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // renderer is singleton
    static Renderer& Instance() {
        static Renderer instance;
        return instance;
    }

    protected:
        ~Renderer() = default;
    private:
        Renderer() = default;

        // keep internal ref after render call to make life easier
        std::shared_ptr<Scene> _scene;

        // use these instead of tick timer
        double _timeElapsed = 0.0f;
        double _timeDelta = 0.0f;
        uint32_t _frameCount = 0;

        void SetPerEditorCameraUniforms(std::shared_ptr<Shader> shader, EditorCamera& editorCamera) {
            shader->Bind();
            // vertex uniforms
            shader->SetMat4("uView", LA::inverse(editorCamera.transform.GetTransform()));
            shader->SetMat4("uProjection", editorCamera.GetProjection());
            // fragment uniforms
            shader->SetVec3("uResolution", editorCamera.width, editorCamera.height, 1.0f);
            shader->SetVec3("uCameraPosition", editorCamera.transform.position);
        }

        void SetPerFrameUniforms(std::shared_ptr<Shader> shader) {
            // timing uniforms
            shader->SetFloat("uTime", _timeElapsed);
            shader->SetFloat("uTimeDelta", _timeDelta);
            shader->SetInt("uFrame", _frameCount);
        }

        void SetPerSceneUniforms(std::shared_ptr<Shader> shader) {
            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);
            ShaderSpotLights(shader);
        }

        void ShaderDirectionalLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = _scene->GetEntitiesWith<DirectionalLightComponent>();
            
            // directional lights
            if (entities.size() >= DIRECTIONAL_LIGHT_MAX) {
                std::cout << "WARNING (Renderer): Too many directional lights, only first 4 will be used" << std::endl;
            }
            for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    DirectionalLightComponent& dlc = entities[i].GetComponent<DirectionalLightComponent>();
                    shader->SetVec3("uDirectionalLights" + index + ".colour", dlc.colour);
                    shader->SetFloat("uDirectionalLights" + index + ".intensity", dlc.intensity);
                    shader->SetVec3("uDirectionalLights" + index + ".direction", tc.GetForward());
                    shader->SetInt("uDirectionalLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("uDirectionalLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderPointLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = _scene->GetEntitiesWith<PointLightComponent>();

            // point lights
            if (entities.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Renderer): Too many point lights, only first 16 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    PointLightComponent& plc = entities[i].GetComponent<PointLightComponent>();
                    shader->SetVec3("uPointLights" + index + ".colour", plc.colour);
                    shader->SetFloat("uPointLights" + index + ".intensity", plc.intensity);
                    shader->SetVec3("uPointLights" + index + ".position", tc.position);
                    shader->SetInt("uPointLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("uPointLights" + index + ".enabled", 0);
                }
            }
        }

        void ShaderSpotLights(std::shared_ptr<Shader> shader) {
            std::vector<Entity> entities = _scene->GetEntitiesWith<SpotLightComponent>();

            // point lights
            if (entities.size() >= POINT_LIGHT_MAX) {
                std::cout << "WARNING (Renderer): Too many spot lights, only first 4 will be used" << std::endl;
            }
            for (int i = 0; i < POINT_LIGHT_MAX; i++) {
                std::string index = "[" + std::to_string(i) + "]";
                if (i < entities.size()) {
                    TransformComponent& tc = entities[i].GetComponent<TransformComponent>();
                    SpotLightComponent& slc = entities[i].GetComponent<SpotLightComponent>();
                    shader->SetVec3("uSpotLights" + index + ".colour", slc.colour);
                    shader->SetFloat("uSpotLights" + index + ".intensity", slc.intensity);
                    shader->SetVec3("uSpotLights" + index + ".position", tc.position);
                    shader->SetVec3("uSpotLights" + index + ".direction", tc.GetForward());
                    shader->SetFloat("uSpotLights" + index + ".cutOff", LA::Radians(slc.cutOff));
                    shader->SetFloat("uSpotLights" + index + ".outerCutOff", LA::Radians(slc.outerCutOff));
                    shader->SetInt("uSpotLights" + index + ".enabled", 1);
                } else {
                    shader->SetInt("uSpotLights" + index + ".enabled", 0);
                }
            }
        }

};