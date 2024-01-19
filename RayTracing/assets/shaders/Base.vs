#version 330 core

layout(location = 0) in vec3 aPos;

out vec2 screenCoord;


struct Ray{
    vec3 origin;
    vec3 direction;
};

Ray CreateRay(vec3 o, vec3 d){
    Ray ray;
    ray.origin = o;
    ray.direction = d;
    
    return ray;
}

vec3 GetRayLocation(Ray ray, float t){
    return ray.origin + t * ray.direction;
}

struct Camera {
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
}; 

uniform Camera camera;

void main(){
    screenCoord = (vec2(aPos.x, aPos.y) + 1.0) / 2.0;
    gl_Position = vec4(aPos, 1.0f);
}