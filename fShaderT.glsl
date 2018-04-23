#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 drawColor;
uniform sampler2D texture1;

void main() {
    // ICE: Change the fragment shader so it makes the background
    // of the font bitmap transparent

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
    FragColor = texture(texture1, TexCoord)*sampled;
}
