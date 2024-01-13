#version 330 core

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

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

// uniforms
uniform vec3    iResolution;
uniform float   iTime;
uniform float   iTimeDelta;
uniform int     iFrame;
uniform vec3    iCameraPosition;
uniform DirectionalLight iDirectionalLights[DIRECTIONAL_LIGHT_MAX];
uniform PointLight iPointLights[POINT_LIGHT_MAX];

// variables passed on from vertex shader
in vec3 fPosition;
in vec3 fNormal;
in vec2 fUV;

// output
out vec4 oColour;

// function declaration
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 norm = normalize(fNormal);
    vec3 viewDir = normalize(iCameraPosition - fPosition);
    vec3 result = vec3(0.0);
    // phase 1: directional lighting
    for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++)
        if (iDirectionalLights[i].enabled != 0)
            result += CalcDirectionalLight(iDirectionalLights[i], norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < POINT_LIGHT_MAX; i++)
        if (iPointLights[i].enabled != 0)
            result += CalcPointLight(iPointLights[i], norm, fPosition, viewDir); 
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
    float attenuation = 1.0 / (light.intensity + 0.09 * distance + 0.032 * (distance * distance));    
    // combine results
    vec3 ambient = light.colour * amb;
    vec3 diffuse = light.colour * diff; 
    vec3 specular = light.colour * spec; 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}