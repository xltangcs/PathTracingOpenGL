#version 330 core

out vec4 FragColor;
uniform samplerBuffer TexturePassData;

void main()
{
    int offset = 0;
    vec3 color = texelFetch(TexturePassData, offset).xyz;

    FragColor = vec4(color, 1.0);
}
