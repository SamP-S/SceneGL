#pragma once

#include "renderer/renderer.hpp"
#include "core/tick_timer.hpp"
#include "serializer/serializer.hpp"

//// TODO:
// consolidate naming such that Tick is every frame/on timer

class RuntimeController {
    public:
        // debug systems
        std::shared_ptr<TickTimer> tickTimer = std::make_shared<TickTimer>();
        std::shared_ptr<EditorCamera> editorCamera = std::make_shared<EditorCamera>();

        // Ngine
        Ngine::AssetManager& assetManager = Ngine::AssetManager::Instance();
        Ngine::AssetLoaderManager& loaderManager = Ngine::AssetLoaderManager::Instance();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();

        // Renderer
        std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
        

        RuntimeController() {}
        ~RuntimeController() {}

        void Initialise() {
            // engine Intialisation
            renderer->scene = scene;
            renderer->tickTimer = tickTimer;
            renderer->editorCamera = editorCamera;
            renderer->Initialise();

            // load scene
            LoadScene("marathon/assets/scenes/Preset.json");    

            // setup editor camera
            editorCamera->transform.position = LA::vec3{-8,5,8};
            editorCamera->transform.rotation = LA::vec3({PI/2, PI, 0.0f});
        }

        void Tick() {
            // update order important
            editorCamera->Update();
            renderer->Render();

            // tick last to setup correct delta
            tickTimer->Tick();
        }

        void Shutdown() {

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

};
