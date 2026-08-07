//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out mat3 TNB;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords * 100;

    vec3 aTangent = vec3(1.0f, 0.0f, 0.0f);
    vec3 aBitangent = vec3(0.0f, 1.0f, 0.0f);

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

    TNB = mat3(T, B, N);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
//#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TNB;

uniform float materialAmbient;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 lightColor;
uniform vec3 lightDir;

struct SpotLight {
    vec3  color;
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

#define MAX_LIGHT_SOURCES 16
uniform int num_of_light_sources;
uniform PointLight lights[MAX_LIGHT_SOURCES];
uniform samplerCube shadow_point_map[MAX_LIGHT_SOURCES];
uniform SpotLight light;
uniform float currentTime;

uniform bool enableAmbient;
uniform bool enableDirectional;
uniform bool enableSpot;
uniform bool enablePoint;
uniform bool enableAnimated;
uniform bool enablePointShadow;

vec3 normal;
vec3 color;

float samplePointShadowMap(int index, vec3 direction)
{
    float depth;
    if (index == 0)
    depth = texture(shadow_point_map[0], direction).r;
    else if (index == 1)
    depth = texture(shadow_point_map[1], direction).r;
    else if (index == 2)
    depth = texture(shadow_point_map[2], direction).r;
    else if (index == 3)
    depth = texture(shadow_point_map[3], direction).r;
    else if (index == 4)
    depth = texture(shadow_point_map[4], direction).r;
    else if (index == 5)
    depth = texture(shadow_point_map[5], direction).r;
    else if (index == 6)
    depth = texture(shadow_point_map[6], direction).r;
    else if (index == 7)
    depth = texture(shadow_point_map[7], direction).r;
    else if (index == 8)
    depth = texture(shadow_point_map[8], direction).r;
    else if (index == 9)
    depth = texture(shadow_point_map[9], direction).r;

    return depth;
}
float calculatePointShadow(int i)
{
    vec3 sampleOffsetDirections[20] = vec3[](
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    vec3 fragToLight = FragPos - lights[i].position;
    float currentDepth = length(fragToLight);
    float bias = 0.2;
    float shadow = 0.0;
    int samples = 10;
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = 0.05;

    for(int j = 0; j < samples; j++)
    {
        float closestDepth = samplePointShadowMap(i, fragToLight + sampleOffsetDirections[j] * diskRadius);
        closestDepth *= 25.0; // far_plane
        if(currentDepth - bias > closestDepth)
        shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

void getAnimatedWaterData(out vec3 finalNorm, out vec3 finalTexColor) {

    float speed = 0.3;

    vec2 uv1 = TexCoords + vec2(currentTime * speed * 0.1f, currentTime * speed * 0.1f);
    vec2 uv2 = TexCoords + vec2(-currentTime * speed * 0.1f, currentTime * speed * 0.7 * 0.1f);
    float wave = sin(TexCoords.x * 0.08 + currentTime) * cos(TexCoords.y * 0.08 + currentTime);
    uv1 += wave * 0.02;
    uv2 += wave * 0.02;

    finalTexColor = texture(texture_diffuse1, uv1).rgb * 0.5f + texture(texture_diffuse1, uv2).rgb * 0.5f;

    vec3 normalSample1 = texture(texture_normal1, uv1).rgb * 2.0 - 1.0;
    vec3 normalSample2 = texture(texture_normal1, uv2).rgb * 2.0 - 1.0;

    vec3 localNormal = normalize(normalSample1 + normalSample2);
    finalNorm = normalize(TNB * localNormal);
}
vec3 calculateDirectionalLight() {

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * color);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    vec3 specular = lightColor * (spec * materialSpecular);

    return diffuse + specular;
}

vec3 calculateSpotLight() {

    vec3 spotLightColor = light.color;

    //diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLightColor * diff * color;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
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

    float shadow = enablePointShadow ? calculatePointShadow(i): 0.0f;

    vec3 lightColor = lights[i].color;
    //diffuse
    vec3 lightDir = normalize(lights[i].position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * color;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    vec3 specular = lightColor * spec * materialSpecular;

    // attenuation
    float distance = length(lights[i].position - FragPos);
    float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
    diffuse *= attenuation;
    specular *= attenuation;
    return (1 - shadow ) * (diffuse + specular);
}
void main() {

    normal = normalize(TNB * (texture(texture_normal1, TexCoords).rgb * 2.0 - 1.0));
    color = texture(texture_diffuse1, TexCoords).rgb;
    if (enableAnimated)
        getAnimatedWaterData(normal, color);

    vec3 result = vec3(0.0f);
    if (enableAmbient)
        result += materialAmbient * color;
    if (enableDirectional)
        result += calculateDirectionalLight();
    if (enablePoint)
        for (int i = 0; i < num_of_light_sources; i++)
            result += calculatePointLight(i);
    if (enableSpot)
        result += calculateSpotLight();

    FragColor = vec4(result, 1.0);

    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));

    BrightColor = brightness > 1.0 ? vec4(result, 1.0): vec4(0.0, 0.0, 0.0, 1.0);
}

