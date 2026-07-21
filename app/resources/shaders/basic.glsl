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
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 materialAmbient;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightDir;

void main() {
    vec3 lightColor = vec3(1.0f, 0.8f, 0.8f);

    //ambient
    vec3 ambient = materialAmbient * texture(texture_diffuse1, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * texture(texture_diffuse1, TexCoords).rgb);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightColor * (spec * materialSpecular);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}
