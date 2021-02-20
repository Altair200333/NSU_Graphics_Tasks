#version 330
out vec4 fragColor;

in vec2 texcoord;
uniform sampler2D background;

const float PI = 3.14159265359;

uniform vec3 origin;
uniform vec3 front;
uniform vec3 right;
uniform vec3 up;
uniform float fov;
uniform float aspectRatio;

uniform int width;
uniform int height;

vec3 getDirection()
{

   float h = tan(fov/2 * PI / 180);
   float l = h*aspectRatio;

   vec3 r = right * (texcoord.x - 0.5)*l;
   vec3 u = up * (texcoord.y - 0.5)*h;
   
   vec3 direction = front + r + u;
   return direction;
}

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
void main() 
{
   vec3 direction = normalize(getDirection());
   vec2 uv = SampleSphericalMap(direction);
   //uv = uv*vec2(2*PI, PI);

   vec3 color = texture(background, uv).rgb;

   fragColor = vec4(color, 1.0f);
}