#version 150 core

in vec3 f_colour;
in vec3 f_normal_coord;

out vec4 o_colour;

void main()
{
	o_colour = vec4(f_normal_coord, 1.0);
}