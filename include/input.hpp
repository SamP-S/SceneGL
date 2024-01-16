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
    private:
        static int _keyStates[SDL_NUM_SCANCODES];
        static int _mouseButtonStates[NUM_MOUSE_BUTTONS];
        static int _mouseX;
        static int _mouseY;

    public:
        // clear state
        static void ClearStates();
        // keyboard events
        static void KeyEvent(int scancode, int state);
        static int GetKeyState(const char* key);
        // mouse events
        static void MouseButtonEvent(int button, int state);
        static void MouseMoved(int x, int y);       
        static int GetMouseButtonState(int button);
        static int GetMouseX();
        static int GetMouseY();
};