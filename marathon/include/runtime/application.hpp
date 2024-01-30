#pragma once

#include <string>
#include <memory>
#include <cassert>

#include "core/tick_timer.hpp"
#include "input/input.hpp"
#include "runtime/operator.hpp"
#include "window/window.hpp"


//// TODO:
// Implement events wrapping/parsing at the highest level before propogating
// Application should not be touched by end user, it is the higher level to the operator
// Application should set project settings
// Application should handle the window manager
// Application should accept an operator that controls the main game engine/game loop
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

    static Application* Create(ApplicationConfig cfg) {
        assert(_instance == nullptr && "Attempting to create application twice. Only 1 allowed.");
        // create new app and return ref
        _instance = new Application(cfg);
        return _instance;
    }

    // Accept externally instanced operator
    void SetOperator(Operator* op) {
        if (op == nullptr) {
            std::cout << "ERROR (Application): Attempting to set operator null." << std::endl;
        }
        _operator = op;
        _operator->Start();
    }
    
    // Application loop, independant of game/sim loop
    void Run() {
        while (!Window.IsOpen()) {
            // poll events
            Window.PollEvents();

            // get time delta
            _tickTimer->Tick();
            double dt = _tickTimer->GetTickElapsed();
            
            // operator tick
            _operator->Update(dt);
            
            // swap frame shown
            Window.SwapFrame();
        }
    }

    // Shutdown modules and free operator
    // Shutdown window context
    ~Application() {
        _operator->End();
        delete _operator;
        Window.Shutdown();
    }

private:
    // Construct application using cfg
    // Initialise context and start tick timer
    Application(ApplicationConfig cfg)
        : _cfg(cfg) {
        Window.Boot();
        // start timer
        _tickTimer->Start();
        // set single app instance
        _instance = this;
    }
    

    ApplicationConfig _cfg;
    std::unique_ptr<TickTimer> _tickTimer = std::make_unique<TickTimer>();
    Operator* _operator = nullptr;
    static inline Application* _instance = nullptr;
};
