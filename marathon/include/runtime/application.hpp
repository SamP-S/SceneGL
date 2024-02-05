#pragma once

#include <string>
#include <memory>
#include <cassert>

#include "core/tick_timer.hpp"
#include "runtime/interactive.hpp"
#include "window/window.hpp"
#include "renderer/renderer.hpp"


//// TODO:
// Implement events wrapping/parsing at the highest level before propogating
// Application should not be touched by end user, it is the higher level to the interactive
// Application should set project settings
// Application should handle the window manager
// Application should accept an interactive that controls the main game engine/game loop
// Allow for dynamic opengl config
// All modules should be boot/shutdown from application

struct ApplicationConfig {
    std::string name = "Example Project";
    std::string cwd = "~";
    OpenGLConfig openglConfig = OpenGLConfig();
    WindowConfig windowConfig = WindowConfig();
};

class Application {
public:

    Window& Window = Window::Instance();
    Renderer& Renderer = Renderer::Instance();

    static Application* Create(ApplicationConfig cfg) {
        assert(_instance == nullptr && "Attempting to create application twice. Only 1 allowed.");
        // create new app and return ref
        _instance = new Application(cfg);
        return _instance;
    }

    // Accept externally instanced interactive
    void SetInteractive(Interactive* op) {
        if (op == nullptr) {
            std::cout << "ERROR (Application): Attempting to set interactive null." << std::endl;
        }
        _interactive = op;
        _interactive->Start();
    }
    
    // Application loop, independant of game/sim loop
    void Run() {
        while (!Window.IsOpen()) {
            // poll events
            Window.PollEvents();

            // get time delta
            _tickTimer->Tick();
            double dt = _tickTimer->GetTickElapsed();
            
            // interactive tick
            _interactive->Update(dt);
            
            // swap frame shown
            Window.SwapFrame();
        }
    }

    // Shutdown modules and free interactive
    // Shutdown window context
    ~Application() {
        _interactive->End();
        delete _interactive;
        Renderer.Shutdown();
        Window.Shutdown();
    }

private:
    // Construct application using cfg
    // Initialise context and start tick timer
    Application(ApplicationConfig cfg)
        : _cfg(cfg) {
        Window.Boot();
        Renderer.Boot();
        // start timer
        _tickTimer->Start();
        // set single app instance
        _instance = this;
    }
    

    ApplicationConfig _cfg;
    std::unique_ptr<TickTimer> _tickTimer = std::make_unique<TickTimer>();
    Interactive* _interactive = nullptr;
    static inline Application* _instance = nullptr;
};
