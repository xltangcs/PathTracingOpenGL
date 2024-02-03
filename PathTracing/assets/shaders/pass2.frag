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

vec3 toneMapping(in vec3 c, float limit) {
    float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;
    return c * 1.0 / (1.0 + luminance / limit);
}

void main() {
    float brightness = 2;
    vec3 color = texture2D(texPass0, pix.xy*0.5+0.5).rgb;
    //vec3 color = vec3(0.5, 0.5, 0.5);
    //color.rgb = clamp(color.rgb * brightness, 0.0, 1.0);
    gl_FragData[0] = vec4(color, 1.0);
}

