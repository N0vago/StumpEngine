#version 330 core

out vec4 FragColor;

in vec3 crntPos;

in vec3 Normal;

in vec3 color;

in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 camPos;

#define MAX_LIGHTS 16

struct Light
{
    int   type;
    vec3  position;
    vec3  direction;
    vec3  color;
    float intensity;
    float range;

    float innerCone;
    float outerCone;
};

uniform int   u_LightCount;
uniform Light u_Lights[MAX_LIGHTS];


vec3 CalcDirectionalLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);

    vec3 diffuse  = diff * light.color * light.intensity;
    vec3 specular = spec * light.color * light.intensity;

    return diffuse + specular;
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = light.position - fragPos;
    float distance = length(lightDir);

    if (distance > light.range)
        return vec3(0.0);

    lightDir = normalize(lightDir);

    float attenuation =
        1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);

    vec3 diffuse  = diff * light.color * light.intensity;
    vec3 specular = spec * light.color * light.intensity;

    return (diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));

    float epsilon = light.innerCone - light.outerCone;
    float intensity = clamp((theta - light.outerCone) / epsilon, 0.0, 1.0);

    if (intensity <= 0.0)
        return vec3(0.0);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);

    vec3 diffuse  = diff * light.color * light.intensity;
    vec3 specular = spec * light.color * light.intensity;

    return (diffuse + specular) * intensity;
}

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(camPos - crntPos);

    vec3 lighting = vec3(0.0);
    vec3 albedo   = texture(diffuse0, texCoord).rgb;
    float specMap = texture(specular0, texCoord).r;

    vec3 ambient = 0.1 * albedo;

    for (int i = 0; i < u_LightCount; i++)
    {
        Light light = u_Lights[i];
        vec3 result = vec3(0.0);

        if (light.type == 0)
            result = CalcDirectionalLight(light, norm, viewDir);
        else if (light.type == 1)
            result = CalcPointLight(light, norm, crntPos, viewDir);
        else if (light.type == 2)
            result = CalcSpotLight(light, norm, crntPos, viewDir);

        lighting += result * albedo + result * specMap;
    }

    FragColor = vec4(ambient + lighting, 1.0);
}