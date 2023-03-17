#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class WindowManager {
    public:
        //static int winWidth;
        //static int winHeight;

        int gl_major = 3;
        int gl_minor = 3;
        const char* glsl_version = "#version 420";
        SDL_GLContext gl_context;
        SDL_Window* window;
        int width;
        int height;
        bool isQuit = false;

        WindowManager() : WindowManager(WINDOW_WIDTH, WINDOW_HEIGHT) {
        }

        WindowManager(int _width, int _height) : width(_width), height(_height) {
            // limit minimum window size
            if (_width < WINDOW_WIDTH)
                _width = WINDOW_WIDTH;
            if (_height < WINDOW_HEIGHT)
                _height = WINDOW_HEIGHT;

            // Initialise SDL subsystems
            SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

            // can fail to set version
            // GL 3.3 + GLSL 330
            glsl_version = "#version 330";
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major) != 0)
                gl_major = 0;
            if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor) != 0)
                gl_minor = 0;

            // Create window with graphics context
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
            window = SDL_CreateWindow("SceneGL + ImGUI + OpenGL4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, window_flags);
            gl_context = SDL_GL_CreateContext(window);
            SDL_GL_MakeCurrent(window, gl_context);
            SDL_GL_SetSwapInterval(1); // Enable vsync
            SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

            // get event
            SDL_Event Event;
            SDL_PollEvent(&Event);
        }

        ~WindowManager() {
            SDL_GL_DeleteContext(gl_context);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        void PollEvents() {
            SDL_Event event;
            // SDL_PollEvent returns 1 while there is an event in the queue
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    // Window event
                    case SDL_WINDOWEVENT:
                        break;
                    // Keyboard event
                    case SDL_KEYDOWN:
                        break;
                    case SDL_KEYUP:
                        break;
                    // Mouse button event
                    case SDL_MOUSEBUTTONDOWN:
                        break;
                    case SDL_MOUSEBUTTONUP:
                        break;
                    case SDL_MOUSEWHEEL:
                        break;
                    // Mouse movement event
                    case SDL_MOUSEMOTION:
                        break;
                    // Exit event
                    case SDL_QUIT:
                        isQuit = true;
                        break;
                }
            }
    }

    void SwapBuffers() {
        // SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        // SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        // SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SwapWindow(window);
    }

    void Delay(int ms) {
        SDL_Delay(ms);
    }

};