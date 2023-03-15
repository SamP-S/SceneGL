#pragma once

#include "SDL_scancode.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"

#define KEY_UP 0
#define KEY_DOWN 1
#define BUTTON_UP 0
#define BUTTON_DOWN 1

class Input {
    public:
        static void KeyEvent(int scancode, int state) {
            _keyStates[scancode] = state;
        }

        // static void MouseButtonEvent(int button, int state) {
        //     _mouseButtonStates[button] = state;
        // }

        static int GetKeyState(const char* key) {
            int scancode = SDL_GetScancodeFromName(key);
            return _keyStates[scancode];
        }


        // static int GetMouseButtonState(const char* button) {
        //     return _mouseButtonStates[SDL_GetMouseState(NULL, NULL)]
        // }

        // static int GetMouseButtonState(int button) {
        //     return _mouseButtonStates[button];
        // }


    private:
        static int _keyStates[SDL_NUM_SCANCODES];
        static int _mouseButtonStates[32];
};

int Input::_keyStates[SDL_NUM_SCANCODES] = {KEY_UP};
int Input::_mouseButtonStates[32] = {BUTTON_UP};