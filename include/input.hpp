#pragma once

#include <iostream>

#include "SDL_scancode.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"

#define KEY_UP 0
#define KEY_DOWN 1
#define BUTTON_UP 0
#define BUTTON_DOWN 1

#define NUM_MOUSE_BUTTONS 8

class Input {
    public:
        static void KeyEvent(int scancode, int state) {
            _keyStates[scancode] = state;
        }

        static void MouseButtonEvent(int button, int state) {
            _mouseButtonStates[button] = state;
        }

        static void MouseMoved(int x, int y) {
            // std::cout << "x: " << x << " \ty: " << y << std::endl;
            _mouseX = x;
            _mouseY = y;
        }

        static void ClearStates() {
            for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
                _keyStates[i] = KEY_UP;
            }
            for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
                _mouseButtonStates[i] = BUTTON_UP;
            }
        }

        static int GetKeyState(const char* key) {
            int scancode = SDL_GetScancodeFromName(key);
            return _keyStates[scancode];
        }

        static int GetMouseButtonState(int button) {
            return _mouseButtonStates[button];
        }

        static int GetMouseX() {
            return _mouseX;
        }

        static int GetMouseY() {
            return _mouseY;
        }

    private:
        static int _keyStates[SDL_NUM_SCANCODES];
        static int _mouseButtonStates[NUM_MOUSE_BUTTONS];
        static int _mouseX;
        static int _mouseY;
};

int Input::_keyStates[SDL_NUM_SCANCODES] = {KEY_UP};
int Input::_mouseButtonStates[NUM_MOUSE_BUTTONS] = {BUTTON_UP, BUTTON_UP, BUTTON_UP, BUTTON_UP, BUTTON_UP, BUTTON_UP, BUTTON_UP, BUTTON_UP};
int Input::_mouseX = 0;
int Input::_mouseY = 0;