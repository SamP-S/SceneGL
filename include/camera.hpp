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

        float mouseSens = 0.3f;
        float speed = 0.01f;
        float FOV = 70.0f;

        float horizontalAngle = 0.0f;
        float verticalAngle = 0.0f;

        void SetProjection(float FOV, float window_width, float window_height, float plane_near, float plane_far)
        {
            proj = Perspective(FOV, window_width / window_height, plane_near, plane_far);
        }

        void RotateCamera(double dx, double dy)
        {
            horizontalAngle += mouseSens * float(-dx);
            verticalAngle += mouseSens * float(dy);

            trans.SetRotation(vec3({
                cosf(verticalAngle) * sinf(horizontalAngle),
                sinf(verticalAngle),
                cosf(verticalAngle) * cosf(horizontalAngle)
            }));
            view = trans.GetTransform();
        }

        void Move(int dir)
        {
            vec3 translate = vec3();
            switch (dir) {
                case MOVE_LEFT:
                    translate = trans.GetRight() * speed;
                    break;
                case MOVE_RIGHT:
                    translate = trans.GetRight() * -speed;
                    break;
                case MOVE_UP:
                    translate = trans.GetUp() * -speed;
                    break;
                case MOVE_DOWN:
                    translate = trans.GetUp() * speed;
                    break;
                case MOVE_FORWARD:
                    translate = trans.GetForward() * -speed;
                    break;
                case MOVE_BACKWARD:
                    translate = trans.GetForward() * speed;
                    break;
            };
            trans.Translate(translate);
            view = trans.GetTransform();
        }

        void Update()
        {
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
        }

};