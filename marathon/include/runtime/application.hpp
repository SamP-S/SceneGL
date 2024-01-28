#pragma once

#include <string>
#include <memory>

#include "core/tick_timer.hpp"
#include "runtime/ioperator.hpp"
#include "renderer/context_manager.hpp"


//// TODO:
// Implement events wrapping/parsing at the highest level before propogating
// Application should not be touched by end user, it is the higher level to the operator
// Application should set project settings
// Application should handle the window manager
// Application should accept an operator that controls the main game engine/game loop
// Remove SDL_Event dependency
// Allow for dynamic opengl config

struct ApplicationConfig {
    std::string name = "Example Project";
    std::string cwd = "~";
};

class Application {
public:

    static Application& Create(ApplicationConfig cfg) {
        assert(_instance == nullptr && "Attempting to create application twice. Only 1 allowed.");
        // create new app and return ref
        Application* app = new Application(cfg);
        return *app;
    }

    static void Destroy() {
        assert(_instance != nullptr && "Attempting to destroy null application.");
        delete _instance;
    }

    static Application& Get() {
        assert(_instance != nullptr && "Can't return reference when Application instance is null.");
        return *_instance;
    }

    // General event handler
    // Catch any quit events, propogate anything else
    void OnEvent(SDL_Event& event) {
        if (event.type == SDL_QUIT) {
            _isQuit = true;
            return;
        }
        _operator->OnEvent(event);
    }

    // Accept externally instanced operator
    void SetOperator(IOperator* op) {
        if (op == nullptr) {
            std::cout << "ERROR (Application): Attempting to set operator null." << std::endl;
        }
        _operator = op;
        _operator->OnInitialise();
    }
    
    // Application loop, independant of game/sim loop
    void Run() {
        while (!_isQuit) {
            // poll events
            _contextManager->PollEvents();

            // get time delta
            _tickTimer->Tick();
            double dt = _tickTimer->GetTickElapsed();
            
            // operator tick
            _operator->OnUpdate(dt);
            
            // swap frame shown
            _contextManager->SwapFrame();
        }
    }

    void Close() {
        _isQuit = true;
    }

    void* GetContext() {
        return _contextManager->gl_context;
    }

    SDL_Window* GetWindow() {
        return _contextManager->window;
    }

    OpenGLConfig& GetOpenGLConfig() {
        return _contextManager->gl_cfg;
    }


private:
    // Construct application using cfg
    // Initialise context and start tick timer
    Application(ApplicationConfig cfg)
        : _cfg(cfg) {
        // hard set OpenGL config
        OpenGLConfig gl_cfg;
        _contextManager->OnInitialise(gl_cfg);
        // Attach app event handler to context manager
        _contextManager->AddEventHandler([this](SDL_Event& event) { OnEvent(event); });
        // start timer
        _tickTimer->Start();
        // set single app instance
        _instance = this;
    }
    
    // Shutdown and free operator
    // Shutdown window context
    ~Application() {
        _operator->OnShutdown();
        delete _operator;
        _contextManager->OnShutdown();
    }

    ApplicationConfig _cfg;
    bool _isQuit = false;
    std::unique_ptr<ContextManager> _contextManager = std::make_unique<ContextManager>();
    std::unique_ptr<TickTimer> _tickTimer = std::make_unique<TickTimer>();
    IOperator* _operator = nullptr;
    static inline Application* _instance = nullptr;
};
