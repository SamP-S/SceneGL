#pragma once

#include "ecs/ngine.hpp"
#include "runtime/interactive.hpp"
#include "renderer/renderer.hpp"
#include "core/tick_timer.hpp"
#include "serializer/scene_serializer.hpp"
#include "input/input.hpp"
#include "platform/opengl/opengl_material.hpp"
#include "platform/opengl/opengl_renderer.hpp"
#include "renderer/shader_loader.hpp"
#include "renderer/model_loader.hpp"


#include "first_person_camera.hpp"

//// TODO:
// consolidate naming such that Tick is every frame/on timer

enum class ShadingMode {
    SHADED,
    WIREFRAME,
    SHADED_WIREFRAME
};

class Runtime : public Interactive {
private:
    // global modules
    AssetManager& assetManager = AssetManager::Instance();
    AssetLoaderManager& loaderManager = AssetLoaderManager::Instance();
    Renderer& Renderer = OpenGLRenderer::Instance();
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // local stuff
    FirstPersonCamera firstPersonCamera = FirstPersonCamera();
    ShadingMode shadingMode = ShadingMode::SHADED_WIREFRAME;

    // Load Scene from JSON
    void LoadScene(const std::string& filepath) {
        SceneSerializer js = SceneSerializer(*scene);
        js.Deserialize(filepath);
    }

    // Save scene to JSON
    void SaveScene(const std::string& filepath) {
        SceneSerializer js = SceneSerializer(*scene);
        js.Serialize(filepath);
    }
    

public:
    Runtime() {}
    ~Runtime() {}

    void Start() override {
        // add loaders to asset libary
        loaderManager.AddLoader(std::make_shared<ModelLoader>());
        loaderManager.AddLoader(std::make_shared<ShaderLoader>());

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

        // load scene
        LoadScene("marathon/assets/scenes/Preset.json");
    }

    void Update(double dt) override {
        firstPersonCamera.Update(dt);
        Renderer.Clear();
        shadingMode = ShadingMode::SHADED_WIREFRAME;
        RenderScene(scene, firstPersonCamera);
    }

    void End() override {
        // clean up resources if necessary
    }

    void RenderScene(std::shared_ptr<Scene> scene, FirstPersonCamera& camera) {
        std::vector<std::shared_ptr<Asset>> shaders = assetManager.GetAssets<OpenGLShader>();
        for (auto asset : shaders) {
            // set editor camera uniforms
            std::shared_ptr<Shader> shader = std::dynamic_pointer_cast<Shader>(asset);
            Renderer.SetProjection(camera.GetProjection());
            Renderer.SetView(LA::inverse(camera.transform.GetTransform()));
            shader->Bind();
            shader->SetVec3("uResolution", camera.width, camera.height, 1.0f);
            shader->SetVec3("uCameraPosition", camera.transform.position);

            // set lighting objects
            ShaderDirectionalLights(shader);
            ShaderPointLights(shader);
            ShaderSpotLights(shader);
        }
        // render entities
        std::vector<Entity> renderables = scene->GetEntitiesWith<MeshRendererComponent>();
        for (auto& ent : renderables) {
            RenderObject(ent);
        }
    }

    void RenderObject(Entity entity) {
        TransformComponent& tc = entity.GetComponent<TransformComponent>();
        mat4 model = tc.GetTransform();

        // check meshrenderer mesh is not none
        MeshRendererComponent& mrc = entity.GetComponent<MeshRendererComponent>();
        std::shared_ptr<Mesh> mesh = mrc.mesh;
        std::shared_ptr<Material> material = mrc.material;
        if (mesh == nullptr || material == nullptr) {
            std::cout << "WARNING (GraphicsEngine): Trying to render null mesh/material." << std::endl;
            return;
        }

        Renderer.context->SetDrawMode(DrawMode::FILL);
        // render shaded
        if (shadingMode == ShadingMode::SHADED || shadingMode == ShadingMode::SHADED_WIREFRAME) {
            if (material->IsUsable()) {
                material->Bind();
                Renderer.RenderMesh(material->shader, mrc.mesh, model);
            }
        } 
        
        Renderer.context->SetDrawMode(DrawMode::LINES);
        // render wireframe
        if (shadingMode == ShadingMode::WIREFRAME || shadingMode == ShadingMode::SHADED_WIREFRAME) {
            std::shared_ptr<Shader> shader = assetManager.FindAsset<OpenGLShader>("base");
            Renderer.RenderMesh(shader, mrc.mesh, model);
        }
    }


    void ShaderDirectionalLights(std::shared_ptr<Shader> shader) {
        std::vector<Entity> entities = scene->GetEntitiesWith<DirectionalLightComponent>();
        
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
        std::vector<Entity> entities = scene->GetEntitiesWith<PointLightComponent>();

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
        std::vector<Entity> entities = scene->GetEntitiesWith<SpotLightComponent>();

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
