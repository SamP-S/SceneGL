#version 330 core

// // standard variables
// in int gl_VertexID;
// in int gl_InstanceID;
// in int gl_DrawID; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseVertex; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseInstance; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// out vec4 gl_Position;
// out int gl_PointSize;

// vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aTangent;
layout(location = 3) in vec4 aColour;
layout(location = 4) in vec2 iUV0;
layout(location = 5) in vec2 iUV1;
layout(location = 6) in vec2 iUV2;
layout(location = 7) in vec2 iUV3;

// "varying" variables
out vec3 vPosition;
out vec3 vNormal;
out vec2 vUV;

// uniforms
uniform mat4	uModel;
uniform mat4	uView;
uniform mat4	uProjection;

void main() {
    vPosition = vec3(uModel * vec4(aPosition, 1.0));
    vNormal = aNormal;
    vUV = aUV;
    gl_Position = uProjection * uView * uModel * vec4(uPosition, 1.0f);
}