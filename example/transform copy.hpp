#pragma once

#include "la_extended.h"
using namespace LA;
#include "component.hpp"

class Entity;

class Transform : public Component {
private:
    LA::vec3 _position, _rotation, _scale;

public:
    Transform(Entity& entity);
    Transform(Entity& entity, LA::vec3 position);
    Transform(Entity& entity, LA::vec3 position, LA::vec3 rotation, LA::vec3 scale);

    LA::mat4 GetCoordinateSystem();
    LA::vec3 GetPosition();
    void SetPosition(LA::vec3 position);
    void Translate(float x, float y, float z);
    void Translate(LA::vec3 translate);
    LA::vec3 GetRotation();
    void SetRotation(LA::vec3 rotation);
    void Rotate(float x, float y, float z);
    void Rotate(LA::vec3 rotate);
    LA::vec3 GetScale();
    void SetScale(LA::vec3 scale);
    void Scale(float x, float y, float z);
    void Scale(LA::vec3 scale);
    LA::mat4 GetTransform();
    LA::vec3 GetForward();
    LA::vec3 GetRight();
    LA::vec3 GetUp();
};