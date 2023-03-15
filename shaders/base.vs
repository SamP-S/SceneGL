#version 420 core

// in int gl_VertexID;
// in int gl_InstanceID;
// in int gl_DrawID; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseVertex; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseInstance; // Requires GLSL 4.60 or ARB_shader_draw_parameters
	
layout(location = 0) in vec3 iPosition;
layout(location = 3) in vec3 iColour;

out vec3 fColour;

uniform float   iTime;

uniform mat4	iModel;
uniform mat4	iView;
uniform mat4	iProjection;

// uniform mat4 uTransform;

void main()
{
    float pi = 3.14159;
    fColour = iColour;
    // gl_Position = vec4(iPosition * 0.3 * (cos(iTime) * 0.4 + 1.2), 1.0) * rotationY(pi * cos(iTime) * 0.2) * rotationX(pi * sin(iTime) * 0.3);
	gl_Position = iProjection * iView * iModel * vec4(iPosition, 1.0f);
}