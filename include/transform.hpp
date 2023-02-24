#include "la.hpp"

class Transform {

    public:
        Transform() :
            _position(LA::vec3()), _rotation(LA::vec3()), _scale(LA::vec3()) {};
        Transform(LA::vec3 position) :
            _position(position), _rotation(LA::vec3()), _scale(LA::vec3()) {};
        Transform(LA::vec3 position, LA::vec3 rotation, LA::vec3 scale) 
            :_position(position), _rotation(rotation), _scale(scale) {};

        // Position
        LA::vec3 GetPosition() { return _position; }
        void SetPosition(LA::vec3 position) { this->_position = position; }
        // Rotation
        LA::vec3 GetRotation() { return _rotation; }
        void SetRotation(LA::vec3 rotation) { this->_rotation = rotation; }
        // Scale
        LA::vec3 GetScale() { return _scale; }
        void SetScale(LA::vec3 scale) { this->_scale = scale; }

    private:
        LA::vec3 _position, _rotation, _scale;
};