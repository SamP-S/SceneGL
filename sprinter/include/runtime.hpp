#pragma once

#include "ngine/ngine.hpp"
#include "runtime/ioperator.hpp"
#include "renderer/renderer.hpp"
#include "core/tick_timer.hpp"
#include "serializer/serializer.hpp"

//// TODO:
// consolidate naming such that Tick is every frame/on timer

class Runtime : public IOperator {
private:
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

public:
    Runtime() {}
    ~Runtime() {}

    void OnInitialise() override {
        // engine Intialisation
        renderer.Initialise();

        // load scene
        LoadScene("marathon/assets/scenes/Preset.json");    
    }

    void OnEvent(SDL_Event& event) {
         switch (event.type) {
            case SDL_KEYUP:
                Input::KeyEvent(event.key.keysym.scancode, KEY_UP);
                break;
            case SDL_KEYDOWN:
                Input::KeyEvent(event.key.keysym.scancode, KEY_DOWN);
                break;
            case SDL_MOUSEMOTION:
                Input::MouseMoved(event.motion.x, event.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                Input::MouseButtonEvent(event.button.button, BUTTON_DOWN);
                break;
            case SDL_MOUSEBUTTONUP:
                Input::MouseButtonEvent(event.button.button, BUTTON_UP);
                break;
            default:
                std::cout << "WARNING (Runtime): No event handle for @ " << event.type << std::endl;
                break;
        }
    }

    void OnUpdate(double dt) override {
        renderer.Render(scene, DrawMode::FILL);
        renderer.Render(scene, DrawMode::LINES);
    }

    void OnShutdown() override {

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
