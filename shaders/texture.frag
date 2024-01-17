#version 150 core

in vec3 f_colour;
in vec3 f_texture_coord;

out vec4 o_colour;

uniform sampler2D tex;

uniform vec3 lightColour;

void main()
{
    float ambientStrength = 0.1;
    o_colour = texture(tex, f_texture_coord.xy);
}