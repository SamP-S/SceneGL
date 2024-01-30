#pragma once

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>

#include "core/module.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


//// TODO:
// Abstract renderer to allow for Vulkan, use cfg structs
// Abstract so not SDL specific implementation
// Implement all the window  property get/set
// Rm event handling from here should be put in event class
// Add support for OpenGL 4.6

//// NOTE:
// can add sRGB to OpenGLConfig
// can add SDL_GL_CONTEXT_DEBUG_FLAG for better OpenGL debugging

struct WindowConfig {
    std::string title = "Config Title";

    // window min size
    int minWidth = WINDOW_WIDTH;
    int minHeight = WINDOW_HEIGHT;

    // window initial size
    int width = 1280;
    int height = 720;

    // window moving/sizing
    int resizable = 1;
    int borderless = 0;
    int centred = 1;
    int fullscreen = 0;
};

struct OpenGLConfig {
    int major = 4;
    int minor = 4;
    const char* glsl = "#version 440 core";

    // swap buffers on refresh rate
    int vsync = 1;
    // have front and back buffers to swap
    int doubleBuffering = 1;
    
    // multisample anti-aliasing
    int msaa = 1;
    // samples per pixel (1,2,4,8,16)
    int msaaSamples = 4;

    // depth buffer size (16, 24, 32)
    int depth = 24;
    // stencil buffer size (0, 1, 8)
    int stencil = 8;
};

class Window : public Module {
private:

    // OpenGL properties
    OpenGLConfig _openglConfig;
    WindowConfig _windowConfig;

    // SDL
    // SLD_GLContext == void*
    SDL_GLContext _openglContext;
    SDL_Window* _window;

    bool _isOpen = false;

protected:
    Window() = default;
    ~Window() = default;

public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // renderer is singleton
    static Window& Instance() {
        static Window* instance;
        if (!instance)
            instance = new Window();
        return *instance;
    }

    // setup context
    void Boot() override {

        // Initialise SDL subsystems
        SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

        // OpenGL version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _openglConfig.major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _openglConfig.minor);

        // Set max depth & stencil buffer size
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, _openglConfig.depth);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, _openglConfig.stencil);

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, _openglConfig.doubleBuffering);

        // msaa
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, _openglConfig.msaa);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, _openglConfig.msaaSamples);
    
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        
        _window = SDL_CreateWindow(
            "Example Title",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            _windowConfig.width, _windowConfig.height, 
            window_flags);
        _openglContext = SDL_GL_CreateContext(_window);

        SDL_GL_MakeCurrent(_window, _openglContext);
        SDL_GL_SetSwapInterval(_openglConfig.vsync);
        
        SetWindowMinSize(_windowConfig.minWidth, _windowConfig.minHeight);
        SetWindowSize(_windowConfig.width, _windowConfig.height);

        // Must be done before any opengl call but after opengl context created
        glewExperimental = GL_TRUE;
        glewInit();
    }

    // destroy context
    void Shutdown() override {
        Close();
        SDL_Quit();
    }

    std::string GetName() override {
        return "Window";
    }

    ModuleType GetType() override {
        return ModuleType::WINDOW;
    }

    // must be called at the start of very new frame
    void PollEvents() {
        SDL_Event event;
        // SDL_PollEvent returns 1 while there is an event in the queue
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    Close();
                    return;
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
    }

    // call at the end of the frame to swap displayed buffer
    void SwapFrame() {
        SDL_GL_SwapWindow(_window);
    }


    bool Close() {
        if (_openglContext) {
            SDL_GL_DeleteContext(_openglContext);
            _openglContext = nullptr;
        }
        if (_window) {
            SDL_DestroyWindow(_window);
            _window = nullptr;

            // remove any outstanding window events as window gone now
            SDL_FlushEvent(SDL_WINDOWEVENT);
        }
        _isOpen = false;
    }


    // --- Window Settings ---

    void SetWindowMinSize(int minWidth, int minHeight) {
        SDL_SetWindowMinimumSize(_window, minWidth, minHeight);
    }
    void GetWindowMinSize(int& minWidth, int& minHeight) {
        SDL_GetWindowMinimumSize(_window, &minWidth, &minHeight);
    }

    void SetWindowSize(int width, int height) {
        SDL_SetWindowSize(_window, width, height);
    }
    void GetWindowSize(int& width, int& height) {
        SDL_GetWindowSize(_window, &width, &height);
    }

    void SetCursorCapture(bool capture) {
        SDL_SetRelativeMouseMode((SDL_bool)capture);
    }
    bool GetCursorCapture() {
        return (bool)SDL_GetRelativeMouseMode();
    }

    bool IsOpen() {
        return _isOpen;
    }

};
