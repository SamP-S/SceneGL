
#define KEY_UP 0
#define KEY_DOWN 1
#define BUTTON_UP 0
#define BUTTON_DOWN 1

class Input {
    public:
        void KeyEvent(int key, int state) {
            _keyStates[key] = state;
        }

        void MouseButtonEvent(int button, int state) {
            _mouseButtonStates[button] = state;
        }

        int GetKeyState(int key) {
            return _keyStates[key];
        }

        int GetMouseButtonState(int button) {
            return _mouseButtonStates[button];
        }


    private:
        int _keyStates[256] = {KEY_UP};
        int _mouseButtonStates[32] = {BUTTON_UP};
};