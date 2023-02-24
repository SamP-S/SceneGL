#version 420 core

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

uniform vec3    iResolution;
uniform float   iTime;
uniform float   iTimeDelta;
uniform int     iFrame;  

in vec3 fColour;

out vec4 oColour;

void main()
{
    oColour = vec4(fColour, 1);
}