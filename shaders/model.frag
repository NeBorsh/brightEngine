#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform int numLights;
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outerCutoff;
};

#define MAX_LIGHTS 5
uniform Light lights[MAX_LIGHTS];
uniform Material material;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

float shadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = shadowCalculation(fragPosLightSpace);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    vec4 texDiffuse = texture(texture_diffuse, TexCoords);
    if (texDiffuse.a < 0.1) discard;

    vec3 texColor = texDiffuse.rgb;

    for (int i = 0; i < numLights; i++) {
        vec3 lightDir;
        float attenuation = 1.0;

        if (lights[i].type == 0) {
            lightDir = normalize(-lights[i].direction);
        } else {
            lightDir = normalize(lights[i].position - FragPos);
            float dist = length(lights[i].position - FragPos);
            attenuation = 1.0 / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);
        }

        if (lights[i].type == 2) {
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = lights[i].cutoff - lights[i].outerCutoff;
            float intensity = clamp((theta - lights[i].outerCutoff) / epsilon, 0.0, 1.0);
            attenuation *= intensity;
        }

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].diffuse * texColor;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * lights[i].specular * material.specular;

        vec3 ambient = lights[i].ambient * texColor;

        result += attenuation * (ambient + diffuse + specular * (1.0 - shadow));
    }

    FragColor = vec4(result, 1.0);
}