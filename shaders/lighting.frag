#version 330 core

// // standard variables
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;
// out float gl_FragDepth;

struct DirectionalLight {
    vec3 direction;
    float intensity;
    vec3 colour;
    int enabled;
};
#define DIRECTIONAL_LIGHT_MAX 4

struct PointLight {
    vec3 position;
	float intensity;
    vec3 colour;
    int enabled;
};
#define POINT_LIGHT_MAX 16

// variables passed on from vertex shader
in vec3 vPosition;
in vec3 vfNormal;
in vec2 vUV0;

// uniforms
uniform vec3    uResolution;
uniform float   uTime;
uniform float   uTimeDelta;
uniform int     uFrame;
uniform vec3    uCameraPosition;
uniform DirectionalLight uDirectionalLights[DIRECTIONAL_LIGHT_MAX];
uniform PointLight uPointLights[POINT_LIGHT_MAX];

// output
out vec4 oColour;

// function declaration
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPosition - vPosition);
    vec3 result = vec3(0.0);
    // phase 1: directional lighting
    for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++)
        if (uDirectionalLights[i].enabled != 0)
            result += CalcDirectionalLight(uDirectionalLights[i], norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < POINT_LIGHT_MAX; i++)
        if (uPointLights[i].enabled != 0)
            result += CalcPointLight(uPointLights[i], norm, vPosition, viewDir); 
    // phase 3: spot light
    // result += CalcSpotLight(spotLight, norm, fPosition, viewDir);    
    
    oColour = vec4(result, 1);
}

// calculates the color when using a directional light.
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float amb = 0.1;
    // combine results
    vec3 ambient = light.colour * light.intensity * amb;
    vec3 diffuse = light.colour * light.intensity * diff;
    vec3 specular = light.colour * light.intensity * spec;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ambient shading
    float amb = 0.1;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (1.0 + 0.09 * distance + 0.032 * (distance * distance));    
    // combine results
    vec3 ambient = light.colour * amb;
    vec3 diffuse = light.colour * diff; 
    vec3 specular = light.colour * spec; 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}