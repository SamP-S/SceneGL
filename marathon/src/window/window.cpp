#include "window/window.hpp"

// renderer is singleton
Window& Window::Instance() {
    static Window* instance;
    if (!instance)
        instance = new Window();
    return *instance;
}

// setup context
void Window::Boot() {

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
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Window.cpp: Failed to initialize GLEW" << std::endl;
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
        _isOk = false;
    } else {
        _isOk = true;
    }
}

// destroy context
void Window::Shutdown() {
    Close();
    SDL_Quit();
}

std::string Window::GetName() {
    return "Window";
}

ModuleType Window::GetType() {
    return ModuleType::WINDOW;
}

void* Window::GetGLContext() {
    return _openglContext;
}
SDL_Window* Window::GetWindow() {
    return _window;
}

void Window::AddEventHandler(std::function<void(SDL_Event&)> handler) {
    _eventHandlers.push_back(handler);
}

// must be called at the start of very new frame
void Window::PollEvents() {
    SDL_Event event;
    // SDL_PollEvent returns 1 while there is an event in the queue
    while (SDL_PollEvent(&event)) {
        // pass straight to handlers
        for (auto& handler : _eventHandlers) {
            handler(event);
        }
        // also pass to INPUT module
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
void Window::SwapFrame() {
    SDL_GL_SwapWindow(_window);
}

bool Window::Close() {
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
bool Window::IsOk() {
    return _isOk;
}
bool Window::IsOpen() {
    return _isOpen;
}

OpenGLConfig Window::GetOpenGLConfig() {
    return _openglConfig;
}


void Window::SetWindowMinSize(int minWidth, int minHeight) {
    SDL_SetWindowMinimumSize(_window, minWidth, minHeight);
}
void Window::GetWindowMinSize(int& minWidth, int& minHeight) {
    SDL_GetWindowMinimumSize(_window, &minWidth, &minHeight);
}

void Window::SetWindowSize(int width, int height) {
    SDL_SetWindowSize(_window, width, height);
}
void Window::GetWindowSize(int& width, int& height) {
    SDL_GetWindowSize(_window, &width, &height);
}

void Window::SetCursorCapture(bool capture) {
    SDL_SetRelativeMouseMode((SDL_bool)capture);
}
bool Window::GetCursorCapture() {
    return (bool)SDL_GetRelativeMouseMode();
}
