#pragma once

#include "la_extended.hpp" 

class Camera {
    private:
        float horizontalAngle = 0.0f;
        float verticalAngle = 0.0f;

        void MoveForward()
        {
            from += direction * speed * 0.001f;
            Update();
        }
        void MoveBackward()
        {
            from -= direction * speed * 0.001f;
            Update();
        }
        void MoveRight()
        {
            from += right * speed * 0.001f;
            Update();
        }
        void MoveLeft()
        {
            from -= right * speed * 0.001f;
            Update();
        }
            void MoveUp()
        {
            from += up * speed * 0.001f;
            Update();
        }
        void MoveDown()
        {
            from -= up * speed * 0.001f;
            Update();
        }


    public:
        bool moveForward = false;
        bool moveBackward = false;
        bool moveLeft = false;
        bool moveRight = false;
        bool moveUp = false;
        bool moveDown = false;

        vec3 from = vec3({0.0f, 0.0f, 0.0f});
        vec3 to = vec3({-1.0f, 1.0f, -1.0f});
        vec3 up = vec3({0.0f, 1.0f, 0.0f});
        vec3 right = vec3({1.0f, 0.0f, -1.0f});
        vec3 direction = vec3({-1.0f, 1.0f, -1.0f});

        mat4 view_matrix = mat4();
        mat4 persp_proj_matrix = mat4();
        mat4 ortho_proj_matrix = mat4();
        mat4 matrix = mat4();

        float mouseSens = 10;
        float speed = 100;
        float FOV = 70;

        void SetProjectionMatrix(float window_width, float window_height, float plane_near, float plane_far)
        {
            persp_proj_matrix = Perspective(FOV, window_width / window_height, plane_near, plane_far);
            Update();
        }

        // void SetOrthographicMatrix(float window_width, float window_height, float plane_near, float plane_far)
        // {
        //     ortho_proj_matrix = Orthographic(0, window_width, 0, window_height, plane_near, plane_far);
        //     Update();
        // }

        void RotateCamera(double dx, double dy)
        {
            horizontalAngle += mouseSens * 0.001f * float(-dx);
            verticalAngle += mouseSens * 0.001f * float(dy);

            direction = vec3({
                cosf(verticalAngle) * sinf(horizontalAngle),
                sinf(verticalAngle),
                cosf(verticalAngle) * cosf(horizontalAngle)
            });

            to = from + direction;

            right = vec3({
                sinf(horizontalAngle - PI / 2.0f),
                0,
                cosf(horizontalAngle - PI / 2.0f)
            });

            up = Cross(right, direction);
        }

        // Call every frame to move the camera for each tick
        void Stop()
        {
            moveForward = false;
            moveBackward = false;
            moveRight = false;
            moveLeft = false;
            moveUp = false;
            moveDown = false;
        }

        void Move()
        {
            if (moveForward)
                MoveForward();
            if (moveBackward)
                MoveBackward();
            if (moveRight)
                MoveRight();
            if (moveLeft)
                MoveLeft();
            if (moveUp)
                MoveUp();
            if (moveDown)
                MoveDown();
        }

        void Update()
        {
            RotateCamera(0, 0);
            view_matrix = LookAt(from, to, up);
            matrix = view_matrix * persp_proj_matrix;
        }

};