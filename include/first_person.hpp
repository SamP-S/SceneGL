#pragma once

#include "transform.hpp"
#include "la_extended.h"
using namespace LA;
#include "component.hpp"
#include "input.hpp"
#include "entity.hpp"

#define MODE_STOP 0
#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4
#define MOVE_FORWARD 5
#define MOVE_BACKWARD 6

class FirstPersonController : public Component {

    private:
        float _sens = 5.0f;
        float _speed = 5.0f;

    public:
        mat4 view = mat4();

        bool active = false;
        int x = 0;
        int y = 0;

        FirstPersonController(Entity* entity) :
            Component(entity) {}

        ~FirstPersonController() {}

        std::string Type() {
            return "FirstPersonController";
        }

        void FromJson(json j) {
            _sens = j["sensitivity"];
            _speed = j["speed"];
        }

        json ToJson() {
            json j;
            j["firstPersonController"]["sensitivity"] = _sens;
            j["firstPersonController"]["speed"] = _speed;
            return j;
        }

        std::string ToString() {
            std::string s = "FirstPersonController(";
            s += std::to_string(_sens) + ", " + std::to_string(_speed) + ")";
            return s;
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

        void RotateCamera(int dx, int dy)
        {
            float dxf = 0.1f * _sens * float(dx);
            float dyf = 0.1f * _sens * float(dy);
            transform->Rotate(dyf, dxf, 0.0f);
        }

        void Move(int dir)
        {
            vec3 translate = vec3();
            vec3 right = transform->GetRight();
            right.y = 0.0f;
            right = Normalise(right);
            vec3 forward = transform->GetForward();
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
            transform->Translate(translate);
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
                view = inverse(transform->GetTransform());

            } else {
                active = false;
            }
        }
};