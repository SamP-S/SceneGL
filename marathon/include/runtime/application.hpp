#pragma once

#include <string>
#include <memory>
#include <cassert>

#include "core/tick_timer.hpp"
#include "input/input.hpp"
#include "runtime/operator.hpp"
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
            case SDL_WINDOWEVENT:
                {
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            {   
                                int w = event.window.data1;
                                int h = event.window.data2;
                            }
                            break;
                        default:
                            // std::cout << "DEBUG(Application): Window event handle not implemented." << std::endl;
                            break;
                    }
                }
                break;
            default:
                // std::cout << "WARNING (Runtime): No event handle for @ " << event.type << std::endl;
                break;
        }
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
        while (!_isQuit) {
            // poll events
            _contextManager->PollEvents();

            // get time delta
            _tickTimer->Tick();
            double dt = _tickTimer->GetTickElapsed();
            
            // operator tick
            _operator->Update(dt);
            
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
        _operator->End();
        delete _operator;
        _contextManager->OnShutdown();
    }

    ApplicationConfig _cfg;
    bool _isQuit = false;
    std::unique_ptr<ContextManager> _contextManager = std::make_unique<ContextManager>();
    std::unique_ptr<TickTimer> _tickTimer = std::make_unique<TickTimer>();
    Operator* _operator = nullptr;
    static inline Application* _instance = nullptr;
};
