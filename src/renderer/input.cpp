#include "renderer/input.hpp"

// initialize static variables
int Input::_keyStates[SDL_NUM_SCANCODES] = {KEY_UP};
int Input::_mouseButtonStates[NUM_MOUSE_BUTTONS] = {BUTTON_UP};
int Input::_mouseX = 0;
int Input::_mouseY = 0;

// clear state
void Input::ClearStates() {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        _keyStates[i] = KEY_UP;
    }
    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++) {
        _mouseButtonStates[i] = BUTTON_UP;
    }
}

// keyboard events
void Input::KeyEvent(int scancode, int state) {
    _keyStates[scancode] = state;
}

int Input::GetKeyState(const char* key) {
    int scancode = SDL_GetScancodeFromName(key);
    return _keyStates[scancode];
}

// mouse events
void Input::MouseButtonEvent(int button, int state) {
    _mouseButtonStates[button] = state;
}

void Input::MouseMoved(int x, int y) {
    _mouseX = x;
    _mouseY = y;
}

int Input::GetMouseButtonState(int button) {
    return _mouseButtonStates[button];
}

int Input::GetMouseX() {
    return _mouseX;
}

int Input::GetMouseY() {
    return _mouseY;
}
