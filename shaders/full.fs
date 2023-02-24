#version 150 core
	
in vec3 f_colour;
in vec3 f_texture_coord;
in vec3 f_normal_coord;

in vec3 f_pos;

out vec4 o_colour;

uniform sampler2D tex;
uniform mat4 matrix;

uniform vec3 obj_rotation;

uniform vec3 lightColour;
uniform vec3 lightPos;

uniform vec3 cameraPos;

void main()
{
    // Rotation
    mat3 rot_x = mat3(1, 0, 0, 0, cos(obj_rotation.x), sin(obj_rotation.x), 0, -sin(obj_rotation.x), cos(obj_rotation.x));
    mat3 rot_y = mat3(cos(obj_rotation.y), 0, -sin(obj_rotation.y), 0, 1, 0, sin(obj_rotation.y), 0, cos(obj_rotation.y));
    mat3 rot_z = mat3(cos(obj_rotation.z), sin(obj_rotation.z), 0, -sin(obj_rotation.z), cos(obj_rotation.z), 0, 0, 0, 1);

    // Ambient
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColour;

    // Diffuse
    vec3 normal = normalize(f_normal_coord);

    // Calculate Positions
    vec3 fragPos = f_pos;
    vec3 lightDirection = lightPos - fragPos;
    
    // Calculate angle of incidence
    float brightness = dot(normal, lightDirection) / (length(lightDirection) * length(normal));
    brightness = clamp(brightness, 0, 1);

    vec4 surfaceColour = texture(tex, f_texture_coord.xy);
    o_colour =  vec4(brightness * lightColour * surfaceColour.rgb, surfaceColour.a);
    //o_colour = surfaceColour;
}