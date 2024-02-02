#version 330 core

in vec3 pix;
out vec4 fragColor;

uniform sampler2D lastColor;

void main() {
    fragColor = vec4(texture(lastColor, pix.xy*0.5+0.5).rgb, 1.0);
}
