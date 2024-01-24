#version 330 core

out vec4 FragColor;
in vec2 screenCoord;

#define MAT_LAMBERTIAN  0
#define MAT_METALLIC    1
#define MAT_DIELECTRIC  2

/*************** Random Begin ***********************/
uint m_u = uint(521288629);
uint m_v = uint(362436069);

uint GetUintCore(inout uint u, inout uint v){
    v = uint(36969) * (v & uint(65535)) + (v >> 16);
    u = uint(18000) * (u & uint(65535)) + (u >> 16);
    return (v << 16) + u;
}

float GetUniformCore(inout uint u, inout uint v){
    uint z = GetUintCore(u, v);
    
    return float(z) / uint(4294967295);
}

float GetUniform(){
    return GetUniformCore(m_u, m_v);
}

uint GetUint(){
    return GetUintCore(m_u, m_v);
}

float rand(){
    return GetUniform();
}

vec2 rand2(){
    return vec2(rand(), rand());
}

vec3 rand3(){
    return vec3(rand(), rand(), rand());
}

vec4 rand4(){
    return vec4(rand(), rand(), rand(), rand());
}

/*************** Random End ***********************/

/*************** Define Begin ***********************/
struct HitRecord{
    float t;
    vec3 position;
    vec3 normal;
};

struct Ray{
    vec3 origin;
    vec3 direction;
};

struct Sphere{
    vec3 center;
    float radius;
}; 

struct World{
    int objectCount;
    Sphere objects[10];
};

struct Camera {
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
}; 

/*************** Define End ***********************/

/*************** Uniform Begin ***********************/
uniform Camera camera;
uniform vec2 screenSize;
/*************** Uniform End ***********************/

Ray CreateRay(vec3 o, vec3 d){
    Ray ray;
    ray.origin = o;
    ray.direction = d;
    
    return ray;
}

Ray CameraGetRay(Camera camera, vec2 uv){
    Ray ray = CreateRay(camera.origin, 
        camera.lower_left_corner + 
        uv.x * camera.horizontal + 
        uv.y * camera.vertical - camera.origin);

    return ray;
}

vec3 GetRayLocation(Ray ray, float t){
    return ray.origin + t * ray.direction;
}

Sphere CreateSphere(vec3 center, float radius){
    Sphere sphere;

    sphere.center = center;
    sphere.radius = radius;

    return sphere;
}

bool HitSphere(Sphere sphere, Ray ray, float t_min, float t_max, inout HitRecord hitRecord){
    vec3 oc = ray.origin - sphere.center;
    
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    float delta = b * b - 4 * a * c;

    if(delta > 0.0){
        float temp = (-b - sqrt(delta)) / (2.0 * a);
        if(temp < t_max && temp> t_min){
            hitRecord.t = temp;
            hitRecord.position = GetRayLocation(ray, hitRecord.t);
            hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;
            
            return true;
        }

        temp = (-b + sqrt(delta)) / (2.0 * a);
        if(temp < t_max && temp> t_min){
            hitRecord.t = temp;
            hitRecord.position = GetRayLocation(ray, hitRecord.t);
            hitRecord.normal = (hitRecord.position - sphere.center) / sphere.radius;

            return true;
        }
    }
    
    return false;
}


World CreateWorld(){
    World world;
    world.objectCount = 2;
    world.objects[0] = CreateSphere(vec3(0.0, 0.0, -1.0), 0.5);
    world.objects[1] = CreateSphere(vec3(0.0, -100.5, -1.0), 100.0);

    return world;
}

bool HitWorld(World world, Ray ray, float t_min, float t_max, inout HitRecord rec){
        HitRecord tempRec;
    bool hitanything = false;
    float cloestFar = t_max;//记录最近的击中点的位置

    //最终会获得这条光线击中的最近物体的信息
    for(int i = 0;i < world.objectCount; i++){
        //如果击中物体，则记录击中点的信息，当击中点比之前的击中点更近时更新信息，更远则不做记录
        if(HitSphere(world.objects[i], ray, t_min, cloestFar, tempRec)){
                rec = tempRec;
            hitanything = true;
            cloestFar = rec.t;//更新最近的击中点
        }
    }

    return hitanything;
}

vec3 RayTrace(Ray ray){
    World world = CreateWorld();

    HitRecord hitRecord;

    if(HitWorld(world, ray, 0.001, 1000000.0, hitRecord)){
        return 0.5 * vec3(hitRecord.normal.x + 1, hitRecord.normal.y + 1, hitRecord.normal.z + 1);
    }

    vec3 normalizeDir = normalize(ray.direction);
    float t = 0.5 * (normalizeDir.y + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

vec3 GammaCorrection(vec3 c){
    return pow(c, vec3(1.0 / 2.2));
}

void main(){
    vec3 color = vec3(0.0, 0.0, 0.0);
    int sampleCount = 100;
    for(int i = 0; i < sampleCount; i++){
        Ray ray = CameraGetRay(camera, screenCoord + rand2() / screenSize);
        color += RayTrace(ray);
    }
    color /= sampleCount;

    color = GammaCorrection(color);

    FragColor = vec4(color, 1.0);
    
}