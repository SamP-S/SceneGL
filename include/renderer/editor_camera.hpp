#pragma once

#include "la_extended.h"

#include "ngine/components.hpp"
#include "renderer/input.hpp"

#define MODE_STOP 0
#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4
#define MOVE_FORWARD 5
#define MOVE_BACKWARD 6

class EditorCamera {
    public:
        // camera properties
        float fov = 45.0f;
        float near = 0.1f;
        float far = 100.0f;
        uint32_t width = 800;
        uint32_t height = 600;
        // first person controller
        bool active = false;
        int x = 0;
        int y = 0;
        float sens = 5.0f;
        float speed = 5.0f;
        // transform
        TransformComponent transform = TransformComponent();
        
        EditorCamera() {}
        ~EditorCamera() {}

        LA::mat4 GetProjection() {
            return LA::Perspective(fov, (float)width / (float)height, near, far);
        }

        void RotateCamera(int dx, int dy)
        {
            float dxf = 0.1f * sens * float(dx);
            float dyf = 0.1f * sens * float(dy);
            transform.rotation += LA::vec3({dyf, dxf, 0.0f});
        }

        void Move(int dir)
        {
            LA::vec3 translate = LA::vec3();
            LA::vec3 right = transform.GetRight();
            right.y = 0.0f;
            right = LA::Normalise(right);
            LA::vec3 forward = transform.GetForward();
            forward.y = 0.0f;
            forward = LA::Normalise(forward);
            switch (dir) {
                case MOVE_LEFT:
                    translate = right * -speed * 0.01;
                    break;
                case MOVE_RIGHT:
                    translate = right * speed * 0.01;
                    break;
                case MOVE_UP:
                    translate = LA::vec3({0.0f, 1.0f, 0.0f}) * speed * 0.01;
                    break;
                case MOVE_DOWN:
                    translate = LA::vec3({0.0f, 1.0f, 0.0f}) * -speed * 0.01;
                    break;
                case MOVE_FORWARD:
                    translate = forward * -speed * 0.01;
                    break;
                case MOVE_BACKWARD:
                    translate = forward * speed * 0.01;
                    break;
            };
            transform.position += translate;
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
            } else {
                active = false;
            }
        }
};