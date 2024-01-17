#version 330 core

// in int gl_VertexID;
// in int gl_InstanceID;
// in int gl_DrawID; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseVertex; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseInstance; // Requires GLSL 4.60 or ARB_shader_draw_parameters
	
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec2 iUV;
layout(location = 3) in vec3 iColour;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fUV;

uniform mat4	iModel;
uniform mat4	iView;
uniform mat4	iProjection;

void main() {
    fPosition = vec3(iModel * vec4(iPosition, 1.0));
    // fNormal = mat3(transpose(inverse(iModel))) * iNormal;  
    fNormal = iNormal;
    fUV = iUV;
    gl_Position = iProjection * iView * iModel * vec4(iPosition, 1.0f);
}