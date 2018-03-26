#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 drawColor;
uniform vec4 transparentColor;
uniform sampler2D texture1;

uniform float alpha;

void main() {
    vec4 texColor = texture(texture1, TexCoord);
    vec4 myColor = texColor;

    if (texColor == transparentColor) {
        myColor = vec4(1.0, 0.0, 0.0, alpha);
    }

    FragColor = myColor;
}
