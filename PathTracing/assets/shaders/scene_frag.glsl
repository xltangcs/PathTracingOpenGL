#version 330 core

out vec4 FragColor;
in vec3 pix;

uniform sampler2D gPosition; 

void main()
{
	//vec2 coord = vec2(0.4, 0.4);
	//vec3 color = texture(gPosition, coord).rgb;
	//vec3 color = vec3(1.0, 0.0, 0.0);
	//FragColor = vec4(color, 1.0);

	FragColor = vec4(texture2D(gPosition, pix.xy*0.5+0.5).rgb, 1.0);
}