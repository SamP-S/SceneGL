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

//// TODO:
// Remove SDL dependancy, need to create intermediate layer
// Add joystick support
// Potentiall seperate mouse, keyboard and joystick into seperate managers

class Input {
private:
// initialize static variables
    static inline int _keyStates[SDL_NUM_SCANCODES] = {KEY_UP};
    static inline int _mouseButtonStates[NUM_MOUSE_BUTTONS] = {BUTTON_UP};
    static inline int _mouseX = 0;
    static inline int _mouseY = 0;

public:
    // clear state
    static void ClearStates() {
        for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
            _keyStates[i] = KEY_UP;
        }
        for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
            _mouseButtonStates[i] = BUTTON_UP;
        }
    }

    // keyboard events
    static void KeyEvent(int scancode, int state) {
        _keyStates[scancode] = state;
    }

    static int GetKeyState(const char* key) {
        int scancode = SDL_GetScancodeFromName(key);
        return _keyStates[scancode];
    }

    // mouse events
    static void MouseButtonEvent(int button, int state) {
        _mouseButtonStates[button] = state;
    }

    static void MouseMoved(int x, int y) {
        _mouseX = x;
        _mouseY = y;
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
};