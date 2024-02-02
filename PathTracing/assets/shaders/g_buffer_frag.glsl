#version 330 core

out vec4 gPosition;

in vec3 pix;

void main() 
{
  gPosition = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}