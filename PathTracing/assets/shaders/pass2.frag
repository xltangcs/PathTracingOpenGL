#version 330 core

in vec3 pix;
out vec4 fragColor;

uniform sampler2D texPass0;
uniform sampler2D texPass1;
uniform sampler2D texPass2;
uniform sampler2D texPass3;
uniform sampler2D texPass4;
uniform sampler2D texPass5;
uniform sampler2D texPass6;

void main() {

    //vec3 color = vec3(0.0, 0.0, 1.0);
    //fragColor = vec4(color, 1.0);

    //vec2 coord = vec2(0.5, 0.5);
    //fragColor = vec4(texture(texPass0, coord).rgb, 1.0);

    gl_FragData[0] = vec4(texture(texPass0, pix.xy*0.5+0.5).rgb, 1.0);

}
