#version 150 core
	
in vec3 v_position_wireframe;

uniform mat4 matrix;
uniform mat4 persp;
uniform mat4 view;

uniform vec3 obj_position;
uniform vec3 obj_rotation;
uniform vec3 obj_scale;

void main()
{
    mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
    mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
    mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);
    vec3 new_pos =  v_position_wireframe * rot_x * rot_y * rot_z * obj_scale + obj_position;
    gl_Position = matrix * persp * view * vec4(new_pos, 1.0);
}