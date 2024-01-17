#version 150 core

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

out vec4 oColour;

void main()
{
    oColour = vec4(0.5, 0.3, 0.7, 1.0);
}