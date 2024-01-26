#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <map>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include "ngine/ngine.hpp"
using namespace Ngine;

#include "runtime/runtime_controller.hpp"

#include "renderer/context_manager.hpp"
#include "renderer/components.hpp"
#include "renderer/renderer.hpp"
#include "input/input.hpp"


class Sprinter {
    private:
        // OpenGL properties
        OpenGLConfig gl_cfg;

        // SDL
        ContextManager contextManager  = ContextManager();
        bool isQuit = false;

        // Runtime Controller
        RuntimeController runtimeController = RuntimeController();

        AssetManager&  assetManager = AssetManager::Instance();


    public:
        Sprinter() {
            // save constructor for app configuration
        }

        void Run() {
            // initialise everything at start of run

            // SDL
            contextManager.Initialise(gl_cfg);
            contextManager.AddEventHandler([this](SDL_Event& event) { EventHandler(event); });
            
            Initialise();
            runtimeController.Initialise();

            // main app loop
            while (!isQuit)
            {
                contextManager.HandleEvents();
                NewFrame();

                runtimeController.Tick();
                contextManager.SwapFrame();
            }
            
            runtimeController.Shutdown();
            Shutdown();
            contextManager.Destroy();
        }

        void EventHandler(SDL_Event& event) {
            if (event.type == SDL_QUIT) {
                isQuit = true;
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
            }
        }
        
        void Initialise() {

        }

        void Shutdown() {
 
        }

        void NewFrame() {

        }

};