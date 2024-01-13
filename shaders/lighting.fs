#version 330 core

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

struct DirLight {
    vec3 direction;
    vec3 strength;
    int enabled;
};
#define DIRECTIONAL_LIGHT_MAX 4

// uniforms
uniform vec3    iResolution;
uniform float   iTime;
uniform float   iTimeDelta;
uniform int     iFrame;
uniform vec3    iCameraPosition;
uniform DirLight iDirectionalLights[DIRECTIONAL_LIGHT_MAX];

// variables passed on from vertex shader
in vec3 fPosition;
in vec3 fNormal;
in vec2 fUV;

out vec4 oColour;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{   
    vec3 norm = normalize(fNormal);
    vec3 viewDir = normalize(iCameraPosition - fPosition);
    vec3 result = vec3(0.5);
    // phase 1: directional lighting
    for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++)
        if (iDirectionalLights[i].enabled != 0)
            result += CalcDirLight(iDirectionalLights[i], norm, viewDir);
    // phase 2: point lights
    // for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    oColour = vec4(result, 1);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.strength * 0.1;
    vec3 diffuse = light.strength * diff;
    vec3 specular = light.strength * spec;
    return (ambient + diffuse + specular);
}