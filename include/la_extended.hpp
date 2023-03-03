#pragma once

#include "la.hpp"

namespace LA {

    const float PI = 3.14159;

    mat4 Translate(float x, float y, float z) {
        mat4 to_return = mat4();
        to_return[3][0] = x;
        to_return[3][1] = y;
        to_return[3][2] = z;
        return to_return;
    }
    mat4 Translate(vec3 v) {
        return Translate(v.x, v.y, v.z);
    }

    mat4 RotateX(float angle) {
        mat4 to_return = mat4();
        to_return[1][1] = cos(angle);
        to_return[2][1] = sin(angle);
        to_return[1][2] = -sin(angle);
        to_return[2][2] = cos(angle);
        return to_return;
    }

    mat4 RotateY(float angle) {
        mat4 to_return = mat4();
        to_return[0][0] = cos(angle);
        to_return[0][2] = sin(angle);
        to_return[2][0] = -sin(angle);
        to_return[2][2] = cos(angle);
        return to_return;
    }

    mat4 RotateZ(float angle) {
        mat4 to_return = mat4();
        to_return[0][0] = cos(angle);
        to_return[0][1] = sin(angle);
        to_return[1][0] = -sin(angle);
        to_return[1][1] = cos(angle);
        return to_return;
    }

    mat4 Rotate(float x, float y, float z) {
        return RotateX(x) * RotateY(y) * RotateZ(z);
    }

    mat4 Rotate(vec3 v) {
        return Rotate(v.x, v.y, v.z);
    }

    mat4 Scale(float x, float y, float z) {
        mat4 to_return = mat4();
        to_return[0][0] = x;
        to_return[1][1] = y;
        to_return[2][2] = z;
        to_return[3][3] = 1.0f;
        return to_return;
    }

    mat4 Scale(vec3 v) {
        return Scale(v.x, v.y, v.z);
    }

    mat4 Perspective(float fov, float aspect, float near, float far) {
        mat4 to_return = mat4(0.0f);
        float tan_half_angle = tan(fov * (PI/180) / 2);
        to_return[0][0] = 1 / (aspect * tan_half_angle);
        to_return[1][1] = 1 / (tan_half_angle);
        to_return[2][2] = -(far + near) / (far - near);
        to_return[2][3] = -1;
        to_return[3][2] = -(2 * far * near) / (far - near);
        return to_return;
    }

}
