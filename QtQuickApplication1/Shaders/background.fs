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

struct Ray
{
	vec3 orig, dir;
};

Ray initRay(vec3 dir, vec3 orig)
{
	Ray r;
	r.dir = normalize(dir);
	r.orig = orig;

	return r;
}


void main() 
{

   float h = tan(fov/2 * PI / 180);
   float l = h*aspectRatio;

   vec3 r = right * (texcoord.x - 0.5)*l;
   vec3 u = up * (texcoord.y - 0.5)*h;
   
   vec3 direction = front + r + u;

   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
	float alpha = atan(-direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;

	float x = theta / 360;
	float y = alpha / 180;

   vec3 color = texture(background, vec2(x, y)).xyz;

   fragColor = vec4(color, 1.0f);
}