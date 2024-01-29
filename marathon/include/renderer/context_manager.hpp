#pragma once

#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <functional>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


//// TODO:
// Implement no/double/triple buffering (currently fixed double)
// Abstract renderer to allow for Vulkan, use cfg structs
// Abstract so not SDL specific implementation

struct OpenGLConfig {
    int major = 4;
    int minor = 6;
    const char* glsl = "#version 460 core";

    // boolean repr
    int vsync = 1;
    int doubleBuffering = 1;

    // window min size
    int minWidth = WINDOW_WIDTH;
    int minHeight = WINDOW_HEIGHT;
    // window initial size
    int width = 1280;
    int height = 720;
};

struct Resolution {
    int width = 0;
    int height = 0;
};

class ContextManager {
    std::vector<std::function<void(SDL_Event&)>> _eventHandlers;

public:
    // OpenGL properties
    OpenGLConfig gl_cfg;

    // SDL
    SDL_GLContext gl_context;
    SDL_Window* window;

    SDL_GLContext backup_gl_context;
    SDL_Window* backup_window;

    ContextManager() = default;
    ContextManager(const ContextManager&) = default;

    // setup context
    void OnInitialise(OpenGLConfig gl_cfg) {

        // Initialise SDL subsystems
        SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

        // Core OpenGL 4.6 + GLSL 460
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_cfg.major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_cfg.minor);

        // Set max depth & stencil buffer size
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gl_cfg.doubleBuffering);
    
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        
        window = SDL_CreateWindow(
            "SceneGL + ImGUI + OpenGL4", SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            gl_cfg.width, gl_cfg.height, 
            window_flags);
        gl_context = SDL_GL_CreateContext(window);

        SDL_GL_MakeCurrent(window, gl_context);

        if (gl_cfg.vsync)
            SDL_GL_SetSwapInterval(1);
        else
            SDL_GL_SetSwapInterval(0);

        SDL_SetWindowMinimumSize(window, gl_cfg.minWidth, gl_cfg.minHeight);

        // Initialise GLEW
        // Must be done before any opengl call
        // easier done before application instanced
        glewExperimental = GL_TRUE;
        glewInit();
    }

    // destroy context
    void OnShutdown() {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    // Add event handler for general context event
    void AddEventHandler(std::function<void(SDL_Event&)> handler) {
        _eventHandlers.push_back(handler);
    }

    // must be called at the start of very new frame
    void PollEvents() {
        SDL_Event event;
        // SDL_PollEvent returns 1 while there is an event in the queue
        while (SDL_PollEvent(&event)) {
            for (auto& handler : _eventHandlers) {
                handler(event);
            }
        }
    }

    void CaptureCursor(bool capture) {
        if (capture)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
            SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    // call at the end of the frame to swap displayed buffer
    void SwapFrame() {
        SDL_GL_SwapWindow(window);
    }

    Resolution GetContextSize() {
        Resolution r;
        SDL_GetWindowSize(window, &r.width, &r.height);
        return r;
    }
    
    // Backup current context and window
    // Do this before calling anything that might switch context under the hood
    void BackupContext() {
        SDL_Window* backup_window = SDL_GL_GetCurrentWindow();
        backup_gl_context = SDL_GL_GetCurrentContext();
    }
    
    // Restore last backed up context
    void RestoreContext() {
        SDL_GL_MakeCurrent(backup_window, backup_gl_context);
    }

};
