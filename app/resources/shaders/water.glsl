//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords * 150;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float materialAmbient;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 lightDir;

struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};
struct PointLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};
#define MAX_LIGHT_SOURCES 32
uniform int num_of_light_sources;
uniform PointLight lights[MAX_LIGHT_SOURCES];
uniform SpotLight light;

vec3 calculateDiretionalLight() {

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * texture(texture_diffuse1, TexCoords).rgb);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(-lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), materialShininess);
    vec3 specular = lightColor * (spec * materialSpecular);

    return diffuse + specular;
}

vec3 calculateSpotLight() {

    vec3 spotLightColor = vec3(1.0f, 1.0f, 1.0f);
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLightColor * diff * texture(texture_diffuse1, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), materialShininess);
    vec3 specular = spotLightColor * spec * materialSpecular;

    // spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}
vec3 calculatePointLight(int i) {

    vec3 lightColor = lights[i].color;
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * texture(texture_diffuse1, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), materialShininess);
    vec3 specular = lightColor * spec * materialSpecular;

    // attenuation
    float distance = length(lights[i].position - FragPos);
    float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}
void main() {

    vec3 result = vec3(0.0f);

    result += materialAmbient * texture(texture_diffuse1, TexCoords).rgb;
    result += calculateDiretionalLight();
    for (int i = 0; i < num_of_light_sources; i++)
        result += calculatePointLight(i);
    result += calculateSpotLight();

    FragColor = vec4(result, 1.0);
}

