#pragma once

#include "la_extended.h"
using namespace LA;

#include "component.hpp"

class Entity;

class Transform : public Component {

    public:
        Transform(Entity& entity) :
            Component(entity),
            _position(LA::vec3()), _rotation(LA::vec3()), _scale(LA::vec3(1.0f)) {}

        Transform(Entity& entity, LA::vec3 position) :
            Component(entity),
            _position(position), _rotation(LA::vec3()), _scale(LA::vec3(1.0f)) {}

        Transform(Entity& entity, LA::vec3 position, LA::vec3 rotation, LA::vec3 scale) :
            Component(entity),
            _position(position), _rotation(rotation), _scale(scale) {}

        std::string Type() {
            return "Transform";
        }

        void FromJson(json j) {
            json pos = j["position"];
            json rot = j["rotation"];
            json scl = j["scale"];
            _position = vec3({pos["x"], pos["y"], pos["z"]});
            _rotation = vec3({rot["x"], rot["y"], rot["z"]});
            _scale = vec3({scl["x"], scl["y"], scl["z"]});
        }

        json ToJson() {
            json j;
            j["position"] = {{"x", _position.x}, {"y", _position.y}, {"z", _position.z}};
            j["rotation"] = {{"x", _rotation.x}, {"y", _rotation.y}, {"z", _rotation.z}};
            j["scale"] = {{"x", _scale.x}, {"y", _scale.y}, {"z", _scale.z}};
            return j;
        }

        std::string ToString() {
            std::string s = "Transform";
            return s;
        }

        // Position
        LA::vec3 GetPosition() { return _position; }
        void SetPosition(LA::vec3 position) { 
            std::cout << "SetPos: " << position.x << " " << position.y << " " << position.z << std::endl;
            this->_position = position; }
        void Translate(float x, float y, float z) {
            _position += vec3({x, y, z});
        }
        void Translate(LA::vec3 translate) {
            Translate(translate.x, translate.y, translate.z);
        }
        // Rotation
        LA::vec3 GetRotation() { return _rotation; }
        void SetRotation(LA::vec3 rotation) { this->_rotation = rotation; }
        void Rotate(float x, float y, float z) {
            _rotation += vec3({x, y, z}); 
        }
        void Rotate(LA::vec3 rotate) {
            Rotate(rotate.x, rotate.y, rotate.z);
        }
        // Scale
        LA::vec3 GetScale() { return _scale; }
        void SetScale(LA::vec3 scale) { this->_scale = scale; }
        void Scale(float x, float y, float z) {
           _scale += vec3({x, y, z});
        }
        void Scale(LA::vec3 scale) {
            Scale(scale.x, scale.y, scale.z);
        }

        LA::mat4 GetTransform() {
            return Transformation(_position, _rotation, _scale);
        }

        // negative the z as opengl looks down the z axis
        LA::vec3 GetForward() {
            mat4 total = GetTransform();
            return vec3({ total[2][0], total[2][1], total[2][2] });
        }

        LA::vec3 GetRight() {
            mat4 total = GetTransform();
            return vec3({ total[0][0], total[0][1], total[0][2] });
        }

        LA::vec3 GetUp() {
            mat4 total = GetTransform();
            return vec3({ total[1][0], total[1][1], total[1][2] });
        }

    private:
        LA::vec3 _position, _rotation, _scale;
};