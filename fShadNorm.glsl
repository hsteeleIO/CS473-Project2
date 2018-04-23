#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 drawColor;
uniform float t2Mix;
uniform int activeTex;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    vec4 texColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    if (activeTex == 2) {
        texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), t2Mix);
    }
    else if (activeTex == 1) {
        texColor = texture(texture1, TexCoord);
    }

    FragColor = texColor * drawColor;

}
