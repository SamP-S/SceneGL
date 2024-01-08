#pragma once

#include "transform.hpp"
#include "la_extended.hpp"
#include "component.hpp"
#include "input.hpp"

#define MODE_STOP 0
#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4
#define MOVE_FORWARD 5
#define MOVE_BACKWARD 6

class Camera : public Component {

    private:
        float _sens = 5.0f;
        float _speed = 5.0f;
        float _fov = 45.0f;

        float _near = 0.1f;
        float _far = 100.0f;
    
    public:
        Transform trans = Transform();

        mat4 view = mat4();
        mat4 proj = mat4();

        bool active = false;

        int x = 0;
        int y = 0;

        Camera(void* entity) :
            Component(entity) {}

        ~Camera() {}

        std::string ComponentType() {
            return "Camera";
        }

        float GetFov() {
            return _fov;
        }

        void SetFov(float fov) {
            _fov = fov;
        }

        float GetLookSensitivity() {
            return _sens;
        }
        void SetLookSensitivity(float sens) {
            _sens = sens;
        }

        float GetMovementSpeed() {
            return _speed;
        }
        void SetMovementSpeed(float speed) {
            _speed = speed;
        }

        void SetProjection(float window_width, float window_height)
        {
            proj = Perspective(_fov, window_width / window_height, _near, _far);
        }

        void RotateCamera(int dx, int dy)
        {
            float dxf = 0.1f * _sens * float(dx);
            float dyf = 0.1f * _sens * float(dy);
            trans.Rotate(dyf, dxf, 0.0f);
        }

        void Move(int dir)
        {
            vec3 translate = vec3();
            vec3 right = trans.GetRight();
            right.y = 0.0f;
            right = Normalise(right);
            vec3 forward = trans.GetForward();
            forward.y = 0.0f;
            forward = Normalise(forward);
            switch (dir) {
                case MOVE_LEFT:
                    translate = right * -_speed * 0.01;
                    break;
                case MOVE_RIGHT:
                    translate = right * _speed * 0.01;
                    break;
                case MOVE_UP:
                    translate = vec3({0.0f, 1.0f, 0.0f}) * _speed * 0.01;
                    break;
                case MOVE_DOWN:
                    translate = vec3({0.0f, 1.0f, 0.0f}) * -_speed * 0.01;
                    break;
                case MOVE_FORWARD:
                    translate = forward * -_speed * 0.01;
                    break;
                case MOVE_BACKWARD:
                    translate = forward * _speed * 0.01;
                    break;
            };
            trans.Translate(translate);
        }

        void Update()
        {
            if (Input::GetMouseButtonState(1)) {
                if (Input::GetKeyState("w"))
                    Move(MOVE_FORWARD);
                if (Input::GetKeyState("s"))
                    Move(MOVE_BACKWARD);
                if (Input::GetKeyState("a"))
                    Move(MOVE_LEFT);
                if (Input::GetKeyState("d"))
                    Move(MOVE_RIGHT);
                if (Input::GetKeyState("left ctrl"))
                    Move(MOVE_DOWN);
                if (Input::GetKeyState("space"))
                    Move(MOVE_UP);

                if (!active) {
                    active = true;
                } else {
                    int dx = Input::GetMouseX() - x;
                    int dy = Input::GetMouseY() - y;
                    RotateCamera(dx, dy);
                }
                x = Input::GetMouseX();
                y = Input::GetMouseY();

                view = inverse(trans.GetTransform());
            } else {
                active = false;
            }
        }

};