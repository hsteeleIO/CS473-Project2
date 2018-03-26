#version 330 core
struct Material {
    bool useDiffuseMap;
    bool useSpecularMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct LightInfo {
    vec3 color;
    vec3 pos;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 NormVec;
in vec3 FragPos;

uniform vec3 camPos;

uniform Material mat;
uniform LightInfo light;

void main() {
    vec3 matColor = mat.diffuse;
    if (mat.useDiffuseMap) matColor = vec3(texture(mat.diffuseMap, TexCoord));

    vec3 specColor = mat.specular;
    if (mat.useSpecularMap) specColor = vec3(texture(mat.specularMap, TexCoord));

    // Ambient lighting:
    vec3 ambient = light.ambientStrength * light.color * matColor;

    // Diffuse lighting:
    vec3 norm = normalize(NormVec);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuseStrength * diff * light.color * matColor;

    // Specular lighting
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    vec3 specular = light.specularStrength * spec * light.color * specColor;

    // Total lighting
    vec4 allLight = vec4(ambient + diffuse + specular, 1.0);

    FragColor = allLight;
}
