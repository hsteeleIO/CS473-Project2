#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aDiffuseColor;
layout (location = 4) in vec3 aSpecularColor;
layout (location = 5) in float aShiny;


out vec2 TexCoord;
out vec3 NormVec;
out vec3 FragPos;
out vec3 DiffuseColor;
out vec3 SpecularColor;
out float Shiny;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    NormVec = mat3(transpose(model)) * aNorm;
    DiffuseColor = aDiffuseColor;
    SpecularColor = aSpecularColor;
    Shiny = aShiny;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

}
