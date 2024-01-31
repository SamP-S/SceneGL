#pragma once

// must define before SDL_opengl.h otherwise GLEW breaks
#define GL_VERSION_4_4
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>

#include "core/module.hpp"
#include "input/input.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//// TODO:
// Abstract renderer to allow for Vulkan, use cfg structs
// Abstract so not SDL specific implementation
// Implement all the window  property get/set
// Rm event handling from here should be put in event class
// Add support for OpenGL 4.6
// Convert opengl config to use bools

//// NOTE:
// can add sRGB to OpenGLConfig
// can add SDL_GL_CONTEXT_DEBUG_FLAG for better OpenGL debugging


struct WindowConfig {
    std::string title = "Config Title";
    int minWidth = WINDOW_WIDTH;
    int minHeight = WINDOW_HEIGHT;
    int width = 1280;
    int height = 720;
    int resizable = 1;
    int borderless = 0;
    int centred = 1;
    int fullscreen = 0;
};

struct OpenGLConfig {
    int major = 4;
    int minor = 4;
    const char* glsl = "#version 440 core";
    int vsync = 0;
    int doubleBuffering = 1;
    int msaa = 1;
    int msaaSamples = 4;
    int depth = 24;
    int stencil = 8;
};

class Window : public Module {
private:
    OpenGLConfig _openglConfig;
    WindowConfig _windowConfig;
    SDL_GLContext _openglContext;
    SDL_Window* _window;
    bool _isOpen = false;

protected:
    Window() = default;
    ~Window() = default;

public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    static Window& Instance();
    void Boot() override;
    void Shutdown() override;
    std::string GetName() override;
    ModuleType GetType() override;
    void PollEvents();
    void SwapFrame();
    bool Close();
    void SetWindowMinSize(int minWidth, int minHeight);
    void GetWindowMinSize(int& minWidth, int& minHeight);
    void SetWindowSize(int width, int height);
    void GetWindowSize(int& width, int& height);
    void SetCursorCapture(bool capture);
    bool GetCursorCapture();
    bool IsOpen();
};