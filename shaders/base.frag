#version 330 core

// // standard variables
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;
// out float gl_FragDepth;

// "varying" variables
in vec3 vColour;

// uniforms
uniform vec3    uResolution;
uniform float   uTime;
uniform float   uTimeDelta;
uniform int     uFrame;  

// outputs
out vec4 oColour;

void main()
{
    oColour = vec4(vColour, 1);
}