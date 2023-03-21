#pragma once

#include "transform.hpp"
#include "la_extended.hpp"
#include "input.hpp"

#define MODE_STOP 0
#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4
#define MOVE_FORWARD 5
#define MOVE_BACKWARD 6

class Camera {
    
    public:
        Transform trans = Transform();

        mat4 view = mat4();
        mat4 proj = mat4();

        float mouseSens = 0.5f;
        float speed = 0.05f;
        float FOV = 70.0f;

        bool active = false;

        int x = 0;
        int y = 0;

        void SetProjection(float FOV, float window_width, float window_height, float plane_near, float plane_far)
        {
            proj = Perspective(FOV, window_width / window_height, plane_near, plane_far);
        }

        void RotateCamera(int dx, int dy)
        {
            float dxf = mouseSens * float(dx);
            float dyf = mouseSens * float(dy);
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
                    translate = right * -speed;
                    break;
                case MOVE_RIGHT:
                    translate = right * speed;
                    break;
                case MOVE_UP:
                    translate = vec3({0.0f, 1.0f, 0.0f}) * speed;
                    break;
                case MOVE_DOWN:
                    translate = vec3({0.0f, 1.0f, 0.0f}) * -speed;
                    break;
                case MOVE_FORWARD:
                    translate = forward * -speed;
                    break;
                case MOVE_BACKWARD:
                    translate = forward * speed;
                    break;
            };
            trans.Translate(translate);
        }

        void Update()
        {
            if (Input::GetMouseButtonState(2)) {
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