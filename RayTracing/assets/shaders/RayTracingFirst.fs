#version 330 core

in vec2 screenCoord;

out vec4 FragColor;

void main(){
    float u = screenCoord.x;
    float v = screenCoord.y;

    FragColor = vec4(u, v, 0.2, 1.0);
}